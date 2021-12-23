const fs = require('fs-extra');                                   //File-System module to read from and write to files
const homedir = require('os').homedir();                          //OS module to require user directory (homedir)
const childProcess = require('child_process');                    //child_process module to make sure cache isn't used when JS files are called.
const { getVideoDurationInSeconds } = require('get-video-duration');   //get-video-duration module to get video duartions in seconds


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

//read data from buffer file updateData.json
async function read () {
  var data = await fs.readJson(homedir + '/AppData/Roaming/MFMB/AutoData/updateData.json', { throws: false,})
  write(data)
}

//update contentLength and write it to buffer file
async function write (data) {
  var dur;
  try {
    for(var i = 0; i < data.length; i++){
      try{  
          dur = await getVideoDurationInSeconds(data[i].videolink)
        } catch (err) {
          console.error(err)
        }
        data[i].contentLength = Math.round(dur);
    }
    await fs.writeJson(homedir + '/AppData/Roaming/MFMB/AutoData/updateData.json', data, {spaces: 1 })
  } catch (err) {
    console.error(err)
  }
  runScript(__dirname + '/insertData.js', function (err) {
    if (err) throw err;
    console.log('finished running insertData.js');
  });
}

read();