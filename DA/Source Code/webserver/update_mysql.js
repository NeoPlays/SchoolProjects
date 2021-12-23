const fs = require('fs-extra');                   //File-System module to read from and write to files
const homedir = require('os').homedir();          //OS module to require user directory (homedir)
const childProcess = require('child_process');    //child_process module to make sure cache isn't used when JS files are called.
const mysql = require('mysql');                   //MySQL module to handle MySQL queries
const { promisify } = require('util');                //util module to "promisify" MySQL queries

const settings = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/settings.json');   //Settings from setup (ip address ...)

//select,insert and delete queries for each table
const selectStatements = ["SELECT * from fe_users","SELECT * from media","SELECT * from algorithm","SELECT * from options"];
const insertStatements = ["INSERT INTO fe_users VALUES ? ON DUPLICATE KEY UPDATE company=VALUES(company)",
                          "INSERT INTO media VALUES ? ON DUPLICATE KEY UPDATE CampaignID=VALUES(CampaignID), Active=VALUES(Active), Image=VALUES(Image), BackgroundColor=VALUES(BackgroundColor), WebsiteLink=VALUES(WebsiteLink), VideoLink=VALUES(VideoLink), uid=VALUES(uid), ContentLength=VALUES(ContentLength), PrevSelected=VALUES(PrevSelected)",
                          "INSERT INTO algorithm VALUES ? ON DUPLICATE KEY UPDATE Credits=VALUES(Credits), PlaybackTime=VALUES(PlaybackTime), CalculatedTime=VALUES(CalculatedTime), PrevSelected=VALUES(PrevSelected), uid=VALUES(uid)",
                          "INSERT INTO options VALUES ? ON DUPLICATE KEY UPDATE PriorityMode=VALUES(PriorityMode)"];
const deleteStatements = ["DELETE FROM fe_users where uid = ?","DELETE FROM media where MediaID = ?","DELETE FROM algorithm where AlgorithmID = ?","DELETE FROM options"];

//extern MySQL server connection
var server = mysql.createConnection({
    host: settings.server_IP,
    user: settings.server_MySQLuser,
    password: settings.server_MySQLpassword,
    database: settings.server_MySQLdatabase
});

//local MySQL client connection
var client = mysql.createConnection({
    host: settings.client_IP,           //localhost
    user: settings.client_MySQLuser,
    password: settings.client_MySQLpassword,
    database: settings.client_MySQLdatabase
});

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
//because queries are stored in 2 dim arrays only IDs are compared [i][0] 
function arr_diff (array1, array2) {

    var array = [], diff = [];

    for (var i = 0; i < array1.length; i++) {
        array[array1[i][0]] = true;
    }

    for (var i = 0; i < array2.length; i++) {
        if (array[array2[i][0]]) {
            delete array[array2[i][0]];
        } else {
            array[array2[i][0]] = true;
        }
    }

    for (var k in array) {
        diff.push(k);
    }
    return diff;
}

//converts the array of objects from the mysql query into a 2 drim array
var ObjArrayToTwoDimArray = function(query_result,StatementIndex) {
    var data = [];
    switch(StatementIndex){
        case 0:
            query_result.forEach((users, x) => {
                data[x] = [users.uid,users.company];
            });
        break;
          
        case 1:
            query_result.forEach((media, x) => {
                data[x] = [media.MediaID, media.CampaignID, media.Active, media.Image, media.BackgroundColor, media.WebsiteLink, media.VideoLink, media.uid, media.ContentLength, media.PrevSelected];
            });
            break;
    
        case 2:
            query_result.forEach((algorithm, x) => {
                data[x] = [algorithm.AlgorithmID, algorithm.Credits, algorithm.PlaybackTime, algorithm.CalculatedTime, algorithm.PrevSelected, algorithm.uid];
            });
            break;

        case 3:
            query_result.forEach((option, x) => {
                data[x] = [option.OptionID, option.PriorityMode];
            });
            break;
  }
  return data;
}

//selects data from external server 
var getServerAsync = async function(i) {
    var promisfyserver = promisify(server.query).bind(server);
    var result = await promisfyserver(selectStatements[i]);     //the correct table is selected by the index i
    return ObjArrayToTwoDimArray(result,i);
};

//inserts/updates data from server on client, compares them, and deletes differences
var setClientAsync = async function(i,server_values) {
    var promisfyclient = promisify(client.query).bind(client);
    var ins_message = await promisfyclient(insertStatements[i], [server_values]);
    console.log(ins_message.affectedRows + " Rows checked");

    var sel_ObjArray = await promisfyclient(selectStatements[i]);
    var client_values = ObjArrayToTwoDimArray(sel_ObjArray,i);

    diff = arr_diff(client_values,server_values);
    diff.forEach(async function(item){
        var del_message = await promisfyclient(deleteStatements[i], item);
        console.log(del_message.affectedRows + " Rows deleted\n");
    });
};

var processDataAsync = async function() {
    var data = [];
    for(var i = 0; i<4; i++){
        data = [];
        try {
            data = await getServerAsync(i);
            await setClientAsync(i,data);
          } catch (error) {
            console.log(error);
          }
    }
    server.end(function (err) {
        if (err) {
            return console.log('error:' + err.message);
        }
        console.log('Server-Database connection closed');
    });
    client.end(function (err) {
        if (err) {
            return console.log('error:' + err.message);
        }
        console.log('Client-Database connection closed');
    });
};

processDataAsync();
runScript(__dirname + '/update_media.js', function (err) {
    if (err) throw err;
    console.log('finished running update_media.js');
});

