/* ::::::::::::::::::::::::::::::::::::::::::::::
rapid importer
version: 1.0.β
author: altnoi.
licence: MIT LICENCE.
:::::::::::::::::::::::::::::::::::::::::::::::*/

dbg_MODE = 0;

// Global var =================================================================
var path = null;
var files = null;
var TGTCMP = null;
var frameRate = 24;
var durationSet = 1; 
var step = 1;
var CMPlist_DATE = [];

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

function adjComp(w, h){
  TGTCMP.width = w;
  TGTCMP.height = h;
}

function imp_source(){
  for(var i = 0;i < files.length;i = i + step){
    pass = app.project.importFile(new ImportOptions(File(files[i])));
    if(i == 0 && rdBtn_a.value == true){
      adjComp(pass.width, pass.height);
    }
    setAV(pass, i);
  }
}

function setAV(pass, i){
  var TGT_LAY = TGTCMP.layers.add(pass, durationSet / frameRate);
  TGT_LAY.startTime = ((i / step) * durationSet) / frameRate;
}

function errMSG(errNum){
  if(errNum == 1){
    ErrorInfo.text = "Invaild folder target";
  }else if(errNum == 2){
    ErrorInfo.text = "No files in this folder";
  }else if(errNum == 3){
    ErrorInfo.text = "Target comp is necessary";
  }
}


// Make window ================================================================
var WndObj = new Window("window", "Rapid importer [1.0.β]", [0, 0,500,400]);
var FldGRP = WndObj.add("panel", [10, 10, 480, 60], "Target Folder");
var FldPath = FldGRP.add("statictext",[20, 10, 390, 30], "Folder is not set now");
var CallFld = FldGRP.add("button", [380, 10, 460, 30], "Select Folder");
var CompGRP = WndObj.add("panel", [10, 70, 480, 160], "Target Comp");
var rdBtn_a = CompGRP.add("radiobutton", [20, 10, 390, 30], "create new composition");
var rdBtn_b = CompGRP.add("radiobutton", [20, 30, 390, 50], "use exist composition");
var compList = CompGRP.add("dropdownlist", [20, 50, 360, 70], []);
var renewList = CompGRP.add("button", [380, 50, 460, 70], "Load comp");
// var createComp = CompGRP.add("button", [380, 20, 460, 40], "comp setting");
var stepGRP = WndObj.add("panel", [10, 170, 480, 220], "Frame steps");
var stepET = stepGRP.add("edittext", [20, 10, 190, 30], "1");
var stepTXT = stepGRP.add("statictext", [200, 10, 390, 30], "frames");
var durGRP = WndObj.add("panel", [10, 230, 480, 280], "Duration");
var durET = durGRP.add("edittext", [20, 10, 190, 30], "1");
var durTXT = durGRP.add("statictext", [200, 10, 390, 30], "frames");
var doAct = WndObj.add("button", [400, 350, 480, 390], "Action");
var ErrorInfo = WndObj.add("statictext", [20, 320, 300, 340], "");
var BuildInfo = WndObj.add("statictext",[20, 350, 300, 390], "Build info : 1.0.β");

// Actions ------------------------------------------------
CallFld.onClick = function(){
  path = slFldAct();
  disp_text = path.toString();
  FldPath.text = disp_text.substr(1, disp_text.length - 1);
}

renewList.onClick = function(){
  compList.removeAll();
  CMPlist_DATE = [];
  var inde = 0;
  for(var j = 1;j<=app.project.items.length;j++){
    if(app.project.items[j] instanceof CompItem){
      CMPlist_DATE.push(app.project.items[j]);
      compList.add("item", app.project.items[j].name, inde);
      inde++;
    }
  }
}

rdBtn_a.onClick = function(){
  if(rdBtn_a.value == true){
  compList.enabled = false;
  renewList.enabled = false;
  createComp.enabled = true;
  TGTCMP = "";
  }
}

rdBtn_b.onClick = function(){
  if(rdBtn_b.value == true){
  compList.enabled = true;
  renewList.enabled = true;
  createComp.enabled = false;
  }
}

compList.onChange = function(){
  TGTCMP = CMPlist_DATE[compList.selection.index];
}

doAct.onClick = function(){
  ErrorInfo.text = "";

  step = parseFloat(stepET.text);
  durationSet = parseFloat(durET.text);

  if(rdBtn_a.value == false && rdBtn_b.value == false){
    errMSG(3);
  }else{
    if(path == null){
      errMSG(1);
    }else{
      files = getFiles();
      if(files.length == 0){
        errMSG(2);
      }else{
        if(rdBtn_a.value == true){
          var newcomp = app.project.items.addComp("imported_Comp", 1920, 1080, 1, ((files.length / step) * durationSet) / frameRate, 24);
          TGTCMP = newcomp;
        }
        imp_source();
      }
    }
  }
}


// DEBUG_CODE
if(dbg_MODE == 1){
  path = File("/h/demo");
  disp_text = path.toString();
  FldPath.text = disp_text.substr(1, disp_text.length - 1);
  BuildInfo.text = "Build info : " + dateObj.toString();
}


// Main Program ===============================================================
WndObj.center();
WndObj.show();
