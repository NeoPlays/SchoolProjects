const homedir = require('os').homedir();
var http = require('http');
var childProcess = require('child_process');
var fs = require('fs');

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

const PORT = 5500;

http.createServer(function (req, res) {
    res.setHeader("Content-Type", "application/json");
    res.setHeader("Access-Control-Allow-Origin", "http://localhost:3000");
    res.setHeader("Access-Control-Allow-Headers", "Content-Type");
        
    if(req.url === '/update' && req.socket.remoteAddress == '::1'){
        runScript(__dirname + '/update_mysql.js', function (err) {
            if (err) throw err;
            console.log('finished running update_mysql.js');
        });
        res.writeHead(200);
        res.end();
    }
        
    if(req.url === '/run' && req.socket.remoteAddress == '::1'){
            runScript(__dirname + '/algorithm/algorithm.js', function (err) {
                if (err) throw err;
                console.log('finished running algorithm.js');
            });
            fs.readFile(homedir + '/AppData/Roaming/MFMB/AutoData/autodata.json', "utf8", (err, jsonString) => {
                if (err) {
                  console.log("File read failed:", err);
                  return;
                }
            res.writeHead(200);
            res.end(jsonString);
        });
    }

    if(req.url === '/getAutoData' && req.socket.remoteAddress == '::1'){
        fs.readFile(homedir + '/AppData/Roaming/MFMB/AutoData/autodata.json', "utf8", (err, jsonString) => {
            if (err) {
              console.log("File read failed:", err);
              return;
            }
            res.writeHead(200);
            res.end(jsonString);
          });
    }

    if(req.url === '/getData' && req.socket.remoteAddress == '::1'){
        fs.readFile(homedir + '/AppData/Roaming/MFMB/AutoData/data.json', "utf8", (err, jsonString) => {
            if (err) {
              console.log("File read failed:", err);
              return;
            }
            res.writeHead(200);
            res.end(jsonString);
          });
    }
    
}).listen(PORT);

console.log("Server started at localhost:" + PORT);

