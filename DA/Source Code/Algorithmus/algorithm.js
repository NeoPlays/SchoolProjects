const fs = require('fs-extra');               //File-System module to read from and write to files
const homedir = require('os').homedir();    //OS module to require user directory (homedir)

var algorithmData = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/algorithmdata.json'); //Data to determine the next selected company which media will be displayed
var mediaData = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/data.json');              //Data to prepare Media-Data-Set for the Auto-Mode
var optionData = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/options.json');          //Data to evaluate if priorityMode is o nor not
var settings = fs.readJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/settings.json');           //Settings from setup (ip address ...)

const prioritizedCompany = mediaData.find(item => item.name == settings.prioritizedCompany);
const priorityMode = optionData[0].priorityMode;        //array for advancements in the future
var prevData = algorithmData.find(item => item.prevSelected == true);

var selection = [];
var media_selection = [];

//Callback function for reduce() function
function sum(total, num) {
    return total + num;
}
//calculated factor for calculatedTime
function weight(credits, sum) {
    return (credits / sum) * 10;
}
//check for priorityMode
if(typeof prevData !== "undefined" && priorityMode && prevData.companyID != prioritizedCompany.companyID){
    selection[0] = prioritizedCompany.companyID;
}else{
//create selection of companies with samllest and same calculatedTime
algorithmData.sort((a, b) => a.calculatedTime - b.calculatedTime);
algorithmData.forEach((data,i,array)=>{                                                  
    if (array[0].calculatedTime == data.calculatedTime) {
        selection.push(data.companyID);
    }
});

//remove previous companyID from the selection
if (typeof prevData !== "undefined") {
    selection.forEach((data,i,array) => {
        if (data == prevData.companyID) {
            array.splice(i, 1);
        }
    });
}
//if selection only consists of the previous selected ID it ends up empty
if (selection.length == 0) {
    algorithmData.forEach((data,i,array) => {
        if (array[1].calculatedTime == data.calculatedTime) {
            selection.push(data.companyID);
        }
    });
}
algorithmData.sort((a, b) => a.companyID - b.companyID);
}

//randomly selected ID of selection
var selectedID = selection[Math.floor(Math.random() * selection.length)];   
console.log("id is " + selectedID);


//create selection of media the company has activated
mediaData.forEach(function (media){
    if(media.companyID == selectedID){media_selection.push(media)}
});


//if there is more than one active, remove the previous one from selection
if(media_selection.length > 1){
    var prevMedia = media_selection.find(item => item.prevSelected == true);
    if (typeof prevMedia !== "undefined") {
        media_selection.forEach((media,i,array) => {
            if (media.id == prevMedia.id) {
                array.splice(i, 1);
            }
        });
    }
}

//randomly select media from media selection
var media = media_selection[Math.floor(Math.random() * media_selection.length)];

//update prevSelected flags
if (typeof prevMedia !== "undefined") {
    mediaData[mediaData.findIndex(item => item.prevSelected == true && item.companyID == prevMedia.companyID)].prevSelected = 0;
}
mediaData[mediaData.findIndex(item => item.companyID == media.companyID && item.campaignID == media.campaignID)].prevSelected = 1;

if (typeof prevData !== "undefined") {
    algorithmData[algorithmData.findIndex(item => item.prevSelected == true)].prevSelected = 0;
}
algorithmData[algorithmData.findIndex(item => item.companyID == selectedID)].prevSelected = 1;

//update calculatedTime and playbackTime
//if priorityMode is active the calculatedTime is adjusted for the prioritized Company
var index = algorithmData.findIndex(item => item.companyID == selectedID);
if(typeof prevData !== "undefined" && priorityMode && prevData.companyID != prioritizedCompany.companyID){
    algorithmData[index].calculatedTime += Math.round(media.contentLength / (Math.round(algorithmData.length/2) * weight(algorithmData[index].credits, algorithmData.map(a => a.credits).reduce(sum))));
}else{
    algorithmData[index].calculatedTime += Math.round(media.contentLength / weight(algorithmData[index].credits, algorithmData.map(a => a.credits).reduce(sum)));
}
algorithmData[index].playbackTime += media.contentLength;

//write updated Data to files
fs.writeJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/algorithmdata.json', algorithmData, {spaces:1});
fs.writeJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/data.json', mediaData, {spaces:1});
//shift array so selected Media is on 4th position (Auto-Mode)
while (mediaData[4].companyID != selectedID) {
    mediaData.unshift(mediaData.pop());
}
fs.writeJsonSync(homedir + '/AppData/Roaming/MFMB/AutoData/autodata.json', mediaData.slice(0, 9), {spaces:1});

