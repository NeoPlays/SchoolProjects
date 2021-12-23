const fs = require('fs-extra');                 //File-System module to read from and write to files
const homedir = require('os').homedir();        //OS module to require user directory (homedir)
const childProcess = require('child_process');  //child_process module to make sure cache isn't used when JS files are called.
const mysql = require('mysql');                 //MySQL module to handle MySQL queries
const { promisify } = require('util');              //util module to "promisify" MySQL queries
const klawSync = require('klaw-sync');          //klaw-sync to recursivly walk a directory

const settings = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/settings.json');   //Settings from setup (ip address ...)

var server_DocDir = '\\\\' + settings.server_IP + '\\Share\\MFMB\\'
server_DocDir = server_DocDir.split('\\').join('/');
var client_DocDir = homedir + "\\Documents\\MFMB\\";
client_DocDir = client_DocDir.split('\\').join('/');

var MediaData = [];

//local MySQL client connection
var con = mysql.createConnection({
  host: settings.client_IP,           //localhost
  user: settings.client_MySQLuser,
  password: settings.client_MySQLpassword,
  database: settings.client_MySQLdatabase
});

//constructor for Media Object
function Media(id, campaignID, active, image, backgroundColor, website, videolink, companyID, contentLength, prevSelected) {
  this.id = id;
  this.campaignID = campaignID;
  this.active = active;
  this.image = image;
  this.backgroundColor = backgroundColor;
  this.website = website;
  this.videolink = videolink;
  this.companyID = companyID;
  this.contentLength = contentLength;
  this.prevSelected = prevSelected;
}

//constructor for File Object
function File(path,birthtime,fname,type,campaign,company,uid) {
  this.path = path;
  this.birthtime = birthtime;
  this.fname = fname;
  this.type = type;
  this.campaign = campaign;
  this.company = company;
  this.uid = uid;
}

//runs JS file (creates a child process)
function runScript(scriptPath, callback) {

  // keep track of whether callback has been invoked to prevent multiple invocations
  var invoked = false;

  var process = childProcess.fork(scriptPath);

  // listen for errors as they may prevent the exit event from firing
  process.on('error', function (err) {
      if (invoked) return;
      invoked = true;
      callback(err);
  });

  // execute the callback once the process has finished running
  process.on('exit', function (code) {
      if (invoked) return;
      invoked = true;
      var err = code === 0 ? null : new Error('exit code ' + code);
      callback(err);
  });

}

//evaluates the differnce of two arrays
function arr_diff (array1, array2) {
  var array = [], diff = [];
  for (var i = 0; i < array1.length; i++) {
      array[array1[i]] = true;
  }

  for (var i = 0; i < array2.length; i++) {
      if (array[array2[i]]) {
          delete array[array2[i]];
      } else {
          array[array2[i]] = true;
      }
  }
  for (var k in array) {
      diff.push(k);
  }
  return diff;
}

//deletes filename from given path
function removeFilename (path){
  var buffer = path.split('/');
  buffer.pop();
  path = buffer.join('/');
  return path;
}

//formates path of given File object from \\ to /
function format (obj){
  var buffer = obj.path.split('\\');
  while(buffer[0] != 'Companies'){
    buffer.shift();
    }
    return buffer.join('/');
}

//extracts Information out of path (Companies/<company name>/<campaign number>/<media-type>/<filename>)
function getInfo (obj,i){                   //i: 0 = Error; i: 1 = Filename; i: 2 = media-type; i: 3 = Campaign number; i: 4 = Company Name
  if(0<i<5){
    var buffer = format(obj).split('/');
    return buffer[buffer.length - i];
  }else{
    return "error";
  }
}

//compares client and server directory and removes differences
function removeDeletedData (){
  const client_dir = klawSync(client_DocDir, {nofile: true});
  const server_dir = klawSync(server_DocDir, {nofile: true});
  var diff = arr_diff(server_dir.map(obj => format(obj)),client_dir.map(obj => format(obj)));
  diff.forEach(item =>{
    console.log(client_DocDir + item);
    fs.removeSync(client_DocDir + item);
  });
}

//select data from database to complete Dataset before writing to buffer file
var pushData = async function(){
  //get uid from fe_users
    var select = "SELECT * FROM fe_users";
    var promisfyquery = promisify(con.query).bind(con);
    var result = await promisfyquery(select);
    result.forEach(function (company, index){
      clientFiles.forEach(function(file){
          if(company.company == file.company){
            file.uid = company.uid;
          }
      });
});
  //get mediaData and update video and image paths
    select = "SELECT * FROM media";
    result = await promisfyquery(select);
    result.forEach(function (media, index){
      MediaData[index] = new Media(media.MediaID, media.CampaignID, media.Active, media.Image, media.BackgroundColor, media.WebsiteLink, media.VideoLink, media.uid, media.ContentLength, media.PrevSelected);
        clientFiles.forEach(function (file){
          if(file.campaign == MediaData[index].campaignID && file.uid == MediaData[index].companyID){
            if(file.type == "video"){
              MediaData[index].videolink = client_DocDir + file.path;
            }else if(file.type == "image"){
              MediaData[index].image = client_DocDir + file.path;
            }
          }
        });
      });

    con.end(function(err){
      if (err) {
          return console.log('error:' + err.message);
      }
      console.log('Close the database connection.');
      //write data in buffer file updateData.json
      fs.ensureDirSync(homedir + '/AppData/Roaming/MFMB/AutoData');
      fs.writeJSONSync(homedir + '/AppData/Roaming/MFMB/AutoData/updateData.json',MediaData,{spaces:1});
  });
};


//walk server direcotry and save every file as an object
const server = klawSync(server_DocDir, {nodir: true});
var serverFiles = [];
server.forEach(function (obj, index){
  serverFiles[index] = new File(format(obj),obj.stats.birthtime,getInfo(obj,1),getInfo(obj,2),getInfo(obj,3),getInfo(obj,4),0);
});

//copy new directories from server to client
serverFiles.forEach(item =>{
  fs.ensureDirSync(client_DocDir + removeFilename(item.path));            //ensure that directory exists
  var files = fs.readdirSync(client_DocDir + removeFilename(item.path));  //reads all files in direcory
  var length = files.length;                                              //gets the number ob files in directory
  if(!fs.pathExistsSync(client_DocDir + item.path) && length == 0){       //if file does not exist and there is no file 
    fs.copySync(server_DocDir + item.path,client_DocDir + item.path);     //then copy the file from server onto client
  }
});

//walk client directory and save every file as an object
const client = klawSync(client_DocDir, {nodir: true});
var clientFiles = [];
client.forEach(function (obj, index){
  clientFiles[index] = new File(format(obj),obj.stats.birthtime,getInfo(obj,1),getInfo(obj,2),getInfo(obj,3),getInfo(obj,4),0);
});

//check for updated files
serverFiles.forEach(item =>{
  //compares birthtime of files
  if(item.birthtime > clientFiles[clientFiles.findIndex(function(file){ return removeFilename(item.path) == removeFilename(file.path)})].birthtime){
    //copy updated files from server with "buffer-" attribute, remove old File and remove "buffer-" attribute
    fs.copySync(server_DocDir + item.path, client_DocDir + removeFilename(item.path) + "/buffer-" + item.fname);                                            
    fs.removeSync(client_DocDir + clientFiles[clientFiles.findIndex(function(file){ return removeFilename(item.path) == removeFilename(file.path)})].path);
    fs.renameSync(client_DocDir + removeFilename(item.path) + "/buffer-" + item.fname, client_DocDir + item.path)
  }
});

//walk updated client directory and save every file as an object (delete old Dataset)
const client_new = klawSync(client_DocDir, {nodir: true});
var clientFiles = [];
client_new.forEach(function (obj, index){
  clientFiles[index] = new File(format(obj),obj.stats.birthtime,getInfo(obj,1),getInfo(obj,2),getInfo(obj,3),getInfo(obj,4),0);
});


removeDeletedData();
pushData();

//checkContentLength (does not work in built app)
/*
runScript(__dirname + '/checkContentLength.js', function (err) {
  if (err) throw err;
  console.log('finished running checkContentLength.js');
});
*/

runScript(__dirname + '/insertData.js', function (err) {
  if (err) throw err;
  console.log('finished running insertData.js');
});



