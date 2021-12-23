const fs = require('fs-extra');                 //File-System module to read from and write to files
const homedir = require('os').homedir();        //OS module to require user directory (homedir)
const childProcess = require('child_process');  //child_process module to make sure cache isn't used when JS files are called.
const mysql = require('mysql');                 //MySQL module to handle MySQL queries

const settings = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/settings.json');     //Settings from setup (ip address ...)
const MediaData = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/updateData.json');  //read data from buffer file

var con = mysql.createConnection({
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

//insert/update data from buffer file into database
con.connect(function (err) {
    if (err)
        throw err;
    console.log("Connected");
    var query = "INSERT INTO media VALUES ? ON DUPLICATE KEY UPDATE CampaignID=VALUES(CampaignID), Active=VALUES(Active), Image=VALUES(Image), BackgroundColor=VALUES(BackgroundColor), WebsiteLink=VALUES(WebsiteLink), VideoLink=VALUES(VideoLink), uid=VALUES(uid), ContentLength=VALUES(ContentLength), PrevSelected=VALUES(PrevSelected)";
    var values = [];
    MediaData.forEach( function(media,index) {
        values[index] = [media.id,media.campaignID,media.active,media.image,media.backgroundColor,media.website,media.videolink,media.companyID,media.contentLength,media.prevSelected];
    });
    con.query(query, [values], function (err, result) {
        if (err) throw err;
        console.log("Number of records inserted: " + result.affectedRows);
    });
    con.end(function(err){
        if (err) {
            return console.log('error:' + err.message);
        }
        console.log('Close the database connection.');
    });
});

runScript(__dirname + '/algorithm/select.js', function (err) {
    if (err) throw err;
    console.log('finished running select.js');
});