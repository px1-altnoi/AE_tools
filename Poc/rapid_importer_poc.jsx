/* ::::::::::::::::::::::::::::::::::::::::::::::
rapid importer
version: PoC Edition
author: altnoi.
licence: MIT LICENCE.
:::::::::::::::::::::::::::::::::::::::::::::::*/

app.beginUndoGroup("rapid importer");

dbg_MODE = 1;

// Global var =================================================================
var path;
var files;
var TGTCMP = app.project.activeItem;
var frameRate = 24;
var durationSet = 1; 


var dateObj = new Date();

// functions ==================================================================
function slFldAct(){
  var fileObj = Folder.selectDialog("ターゲットフォルダを選択");
  return fileObj;
}

function getFiles(){
  var fileList = path.getFiles();
  return fileList;
}

function imp_source(){
  for(var i = 0;i < files.length;i++){
    pass = app.project.importFile(new ImportOptions(File(files[i])));
    setAV(pass, i);
  }
}

function setAV(pass, i){
  // alert(pass);
  var TGT_LAY = TGTCMP.layers.add(pass, durationSet / frameRate);
  TGT_LAY.startTime = (i + 1) / frameRate;
}


// Make window ================================================================
var WndObj = new Window("window", "Rapid importer [PoC Edition]", [0, 0,500,250]);
var FldPath = WndObj.add("statictext",[20, 10, 180, 60], "Folder is not set now");
var CallFld = WndObj.add("button", [400, 10, 480, 60], "Select Folder");
var doAct = WndObj.add("button", [400, 160, 480, 240], "Action");
var BuildInfo = WndObj.add("statictext",[20, 210, 300, 240], "Build info : " + dateObj.toString());

// Action -------------------------------------------------
CallFld.onClick = function(){
  path = slFldAct();
  FldPath.text = path;
}

doAct.onClick = function(){
  files = getFiles();
  imp_source();
}



WndObj.center();
WndObj.show();

app.endUndoGroup();
