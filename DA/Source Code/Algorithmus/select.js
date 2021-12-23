const fs = require('fs-extra');                     //File-System module to read from and write to files
const homedir = require('os').homedir();    //OS module to require user directory (homedir)
const mysql = require('mysql');               //MySQL module to handle MySQL queries

const settings = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/settings.json'); //Settings from setup (ip address ...)
var resultData = [];

//mysql Connection options
var con = mysql.createConnection({
    host: settings.client_IP,
    user: settings.client_MySQLuser,
    password: settings.client_MySQLpassword,
    database: settings.client_MySQLdatabase
});
//establish mysql connection
con.connect(function (err) {
    if (err)
        throw err;
    console.log("Connected");
    //select query for MediaData (data.json) from media table inner joined to fe_users
    var query = "SELECT media.MediaID, media.Image, fe_users.company, media.BackgroundColor, media.WebsiteLink, media.VideoLink, media.uid, media.CampaignID, media.ContentLength, media.PrevSelected FROM media INNER JOIN fe_users on media.uid=fe_users.uid where media.Active = 1";
    con.query(query, function (err, result, fields) {
        if (err) {
            throw err;
        }
        else {
            result.forEach(function (media, index) {
                resultData[index] = {id: String(media.MediaID - 1), image: media.Image, name: media.company, backgroundColor: media.BackgroundColor, website: media.WebsiteLink, videolink: media.VideoLink, companyID: media.uid-1, campaignID: media.CampaignID, contentLength: media.ContentLength, prevSelected: media.PrevSelected};
            });
            fs.writeJSONSync(homedir + '/AppData/Roaming/MFMB/AutoData/data.json', resultData,{spaces:1});
        }
    });
    //select query for AlgorithmData (algorithmdata.json) from algorithm table
    query = "SELECT algorithm.Credits, algorithm.PlaybackTime, algorithm.CalculatedTime, algorithm.PrevSelected, algorithm.uid FROM algorithm;";
    con.query(query, function (err, result, fields) {
        if (err) {
            throw err;
        }
        else {
            resultData = [];
            result.forEach(function (algorithm, index) {
                resultData[index] = {credits: algorithm.Credits, playbackTime: algorithm.PlaybackTime, calculatedTime: algorithm.CalculatedTime, prevSelected: algorithm.PrevSelected, companyID: algorithm.uid - 1};
            });
            fs.writeJSONSync(homedir + '/AppData/Roaming/MFMB/AutoData/algorithmdata.json', resultData,{spaces:1});
        }
    });
    //select query for optionData (options.json) from options table
    query = "SELECT * FROM options";
    con.query(query, function (err, result, fields) {
        if (err) {
            throw err;
        }
        else {
            resultData = [];
            result.forEach(function (options, index) {
                resultData[index] = {optionID: options.OptionID,priorityMode: options.PriorityMode};
            });
            fs.writeJSONSync(homedir + '/AppData/Roaming/MFMB/AutoData/options.json', resultData,{spaces:1});
        }
    });
    con.end(function (err) {
        if (err) {
            return console.log('error:' + err.message);
        }
        console.log('Close the database connection.');
    });
});
