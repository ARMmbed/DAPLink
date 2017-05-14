const char microbit_html[] = "\n\n\
<html><meta charset=\"UTF-8\">\n\
<head>\n\
<style type='text/css'>\n\
body{font-family:Verdana, sans-serif; font-size:14px;padding:0px;margin:0px;}\n\
hr{background:#AADDBB;color:#AADDBB;height:8px;border:0px;}\n\
#fs{width:100%; text-align: center;}\n\
#fs table{table-layout: auto; width: 100%;vertical-align: middle;text-align: center;}\n\
.navbar{padding:10px;background-color: #000;}\n\
.navbar h2{color:#fff;}\n\
#clipboard{\n\
    max-width: 100%;\n\
    display: block;\n\
    width: 100%;\n\
    height:auto;\n\
    padding: 6px 12px;\n\
    font-size: 14px;\n\
    line-height: 1.42857143;\n\
    color: #555;\n\
    background-color: #fff;\n\
    background-image: none;\n\
    border: 1px solid #ccc;\n\
    border-radius: 4px;\n\
    -webkit-box-shadow: inset 0 1px 1px rgba(0,0,0,.075);\n\
    box-shadow: inset 0 1px 1px rgba(0,0,0,.075);\n\
    -webkit-transition: border-color ease-in-out .15s,-webkit-box-shadow ease-in-out .15s;\n\
    -o-transition: border-color ease-in-out .15s,box-shadow ease-in-out .15s;\n\
    transition: border-color ease-in-out .15s,box-shadow ease-in-out .15s;\n\
}\n\
.popup-wrapper{\n\
    display: none;\n\
    position: absolute;\n\
    top: 0;\n\
    left: 0;\n\
    width: 100%;\n\
    height: 100%;\n\
    z-index: 10;\n\
    background: rgba(0,0,0,0.7);\n\
}\n\
.popup{\n\
    max-height: calc(100% - 100px);\n\
    position: fixed;\n\
    top: 50%;\n\
    left: 50%;\n\
    transform: translate(-50%, -50%);\n\
}\n\
.info-wrapper{\n\
    background-color: #fff;\n\
    border-radius: 15px;\n\
    padding: 5px;\n\
}\n\
a {\n\
    text-decoration: none;\n\
}\n\
.btn{display: inline-block;\n\
    padding: 6px 12px;\n\
    margin-bottom: 0;\n\
    font-size: 14px;\n\
    font-weight: 400;\n\
    line-height: 1.42857143;\n\
    text-align: center;\n\
    white-space: nowrap;\n\
    vertical-align: middle;\n\
    -ms-touch-action: manipulation;\n\
    touch-action: manipulation;\n\
    cursor: pointer;\n\
    -webkit-user-select: none;\n\
    -moz-user-select: none;\n\
    -ms-user-select: none;\n\
    user-select: none;\n\
    background-image: none;\n\
    border: 1px solid transparent;\n\
    border-radius: 4px;}\n\
.btn-default:hover{color: #333;\n\
    background-color: #e6e6e6;\n\
    border-color: #adadad;}\n\
.btn-default{\n\
    color: #333;\n\
    background-color: #fff;\n\
    border-color: #ccc;\n\
}\n\
button, html input[type=button], input[type=reset], input[type=submit] {\n\
    -webkit-appearance: button;\n\
    cursor: pointer;\n\
}\n\
.padded{\n\
    margin:5px;\n\
}\n\
.btn-primary:hover{color: #fff;\n\
    background-color: #286090;\n\
    border-color: #204d74;}\n\
.btn-primary{\n\
    color: #fff;\n\
    background-color: #337ab7;\n\
    border-color: #2e6da4;\n\
}\n\
\n\
.btn-success{\n\
    color: #fff;\n\
    background-color: #5cb85c;\n\
    border-color: #5cb85c;\n\
}\n\
.btn-success:hover{\n\
    color: #fff;\n\
    background-color: #449d44;\n\
    border-color: #419641;\n\
}\n\
.loader {margin: 60px auto;font-size: 10px;position: relative;text-indent: -9999em;border-top: 1.1em solid rgba(0, 0, 0, 0.2);border-right: 1.1em solid rgba(0, 0, 0, 0.2);border-bottom: 1.1em solid rgba(0, 0, 0, 0.2);border-left: 1.1em solid #000;-webkit-transform: translateZ(0);-ms-transform: translateZ(0);transform: translateZ(0);-webkit-animation: load8 1.1s infinite linear;animation: load8 1.1s infinite linear;}\n\
.loader,.loader:after {border-radius: 50%;width: 5em;height: 5em;}\n\
@-webkit-keyframes load8 {0%{-webkit-transform: rotate(0deg);transform: rotate(0deg);}100%{-webkit-transform: rotate(360deg);transform: rotate(360deg);}}\n\
@keyframes load8 {0%{-webkit-transform: rotate(0deg);transform: rotate(0deg);}100%{-webkit-transform: rotate(360deg);transform: rotate(360deg);}}\n\
\n\
/*----- Tabs -----*/\n\
.tabs {\n\
    width:100%;\n\
    display:inline-block;\n\
}\n\
\n\
/*----- Tab Links -----*/\n\
/* Clearfix */\n\
.tab-links:after {\n\
    display:block;\n\
    clear:both;\n\
    content:'';\n\
}\n\
\n\
ul {\n\
    display: table;\n\
    border-spacing: 6px 0px;\n\
    margin:0;\n\
    padding:0;\n\
    width:100%;\n\
    text-align:center;\n\
}\n\
.tab-links li {\n\
    position: relative;\n\
    margin-right: 5px;\n\
    display: table-cell !important;\n\
    vertical-align: middle;\n\
    border:1px solid black;\n\
    border-bottom: none;\n\
    width:28%;\n\
    height:50px;\n\
    margin: 0;\n\
    display:inline-block;\n\
    list-style:none;\n\
    background:#fff;\n\
    border-radius:3px 3px 0px 0px;\n\
    transition:all linear 0.15s;\n\
}\n\
\n\
.tab-links li:after{\n\
    position: absolute;\n\
    content: \"\";\n\
    width: 100%;\n\
    bottom: -1px;\n\
    left: 0;\n\
    border-bottom: 1px solid black;\n\
    z-index: 1;\n\
}\n\
\n\
.tab-links li.active:after {\n\
    bottom:-1px;\n\
    z-index: 2;\n\
    border-bottom: 1px solid #FFF;\n\
}\n\
\n\
.tab-links a {\n\
    text-decoration:none;\n\
    padding:0 10px 0 10px;\n\
    font-size:12px;\n\
    font-weight:600;\n\
    color: #4c4c4c\n\
    /*color:#4c4c4c;*/\n\
}\n\
@media (min-width: 400px){\n\
   .tab-links a {\n\
        text-decoration:none;\n\
        padding:0 10px 0 10px;\n\
        font-size:16px;\n\
        font-weight:600;\n\
        color: #4c4c4c\n\
        /*color:#4c4c4c;*/\n\
    }\n\
    ul {\n\
        display: table;\n\
        border-spacing: 15px 0px;\n\
        margin:0;\n\
        padding:0;\n\
        width:100%;\n\
        text-align:center;\n\
    }\n\
}\n\
\n\
.tab-links a:hover {\n\
    text-decoration:none;\n\
    color:#337ab7;\n\
}\n\
\n\
li.active, li.active:hover, li.active a{\n\
    background:#fff;\n\
    color:#337ab7;\n\
}\n\
\n\
/*----- Content of Tabs -----*/\n\
.tab-content {\n\
    background-color: #fff !important;\n\
    padding:3px;\n\
    border-radius:3px;\n\
    border:solid 1px black;\n\
    background:#fff;\n\
}\n\
\n\
.tab {\n\
    display:none;\n\
}\n\
\n\
.tab.active {\n\
    display:block;\n\
    background-color: #fff !important;\n\
}\n\
\n\
.speech-bubble {\n\
    position: relative;\n\
    padding: 15px;\n\
    margin: 1em 0 3em;\n\
    border: 5px solid black;\n\
    color: #333;\n\
    background: #fff;\n\
    -webkit-border-radius: 10px;\n\
    -moz-border-radius: 10px;\n\
    border-radius: 10px;\n\
}\n\
\n\
.bubble-align {\n\
    margin-left: 30px;\n\
}\n\
\n\
.speech-bubble:before {\n\
    content: \"\";\n\
    position: absolute;\n\
    bottom: -20px;\n\
    left: 40px;\n\
    border-width: 20px 20px 0;\n\
    border-style: solid;\n\
    border-color: #000 transparent;\n\
    display: block;\n\
    width: 0;\n\
}\n\
\n\
.speech-bubble:after {\n\
    content: \"\";\n\
    position: absolute;\n\
    bottom: -13px;\n\
    left: 47px;\n\
    border-width: 13px 13px 0;\n\
    border-style: solid;\n\
    border-color: #fff transparent;\n\
    display: block;\n\
    width: 0;\n\
}\n\
\n\
.bubble-align:before {\n\
    top: 10px;\n\
    bottom: auto;\n\
    left: -30px;\n\
    border-width: 15px 30px 15px 0;\n\
    border-color: transparent #000;\n\
}\n\
\n\
.bubble-align:after {\n\
    top: 16px;\n\
    bottom: auto;\n\
    left: -21px;\n\
    border-width: 9px 21px 9px 0;\n\
    border-color: transparent #fff;\n\
}\n\
\n\
@media (max-width: 667px){\n\
    .bubble-align:before {\n\
        top: -20px;\n\
        bottom: auto;\n\
        right: auto;\n\
        left: 40px;\n\
        border-width: 0 20px 20px;\n\
        border-color: #000 transparent;\n\
    }\n\
\n\
    .bubble-align:after {\n\
        top: -13px;\n\
        bottom: auto;\n\
        right: auto;\n\
        left: 47px;\n\
        border-width: 0 13px 13px;\n\
        border:default;\n\
        border-color: #fff transparent;\n\
    }\n\
}\n\
</style>\n\
<script>\n\
function ascii2string(ascii_ar) {\n\
    var str = \"\";\n\
    for(x in ascii_ar) {\n\
        if(ascii_ar[x] == 0) break;\n\
        str += String.fromCharCode(ascii_ar[x]);\n\
    }\n\
    return str;\n\
}\n\
\n\
function makePageBlob(id, len) {\n\
    var ar = Uint8Array.from(fl.slice(id*1024,(id)*1024+len));\n\
    return new Blob([ar]);\n\
}\n\
\n\
function makeBlob(blocks,size) {\n\
    var fileBlob = new Blob();\n\
    for(x in blocks) {\n\
    if(blocks[x] == 0xFF) return fileBlob;\n\
        fileBlob = new Blob([fileBlob,\n\
            makePageBlob(blocks[x]+1,\n\
            Math.min(1024,size))]);\n\
        size-=1024;\n\
    }\n\
    return fileBlob;\n\
}\n\
\n\
function getFile(id) {\n\
    var ofs = 80*id;\n\
    if(fl[ofs+19] & 0x80) return 0;\n\
\n\
    var size = fl[ofs+16]+\n\
        (fl[ofs+17]<<8)+\n\
        (fl[ofs+18]<<16)+\n\
        (fl[ofs+19]<<24);\n\
\n\
    var obj = {\n\
        file:(new File([makeBlob(fl.slice(ofs+20,ofs+80), size)], name)),\n\
        name:(ascii2string(fl.slice((ofs), (ofs)+14))),\n\
        size:size\n\
    }\n\
    return obj;\n\
}\n\
\n\
function autoCopy(){\n\
    var clipboard = document.querySelector('#clipboard');\n\
    var range = document.createRange();\n\
    range.selectNode(clipboard);\n\
    window.getSelection().addRange(range);\n\
\n\
    var success = false;\n\
\n\
    try {\n\
        var success = document.execCommand('copy');\n\
    } catch(err) {\n\
    }\n\
    window.getSelection().removeAllRanges();\n\
\n\
    return success;\n\
}\n\
\n\
function copyToClipboard(id, button){\n\
    var clipboard = document.getElementById(\"clipboard\")\n\
    clipboard.value = getFile(id).file.blob;\n\
    document.getElementById(\"popup\").style.display=\"block\";\n\
\n\
    if(autoCopy()){\n\
        document.getElementById(\"popup\").style.display=\"none\";\n\
        button.className += \" btn-success\"\n\
        button.disabled = true;\n\
        button.innerHTML = \"Copied!\"\n\
\n\
        setTimeout(function(){\n\
            button.className = button.className.replace(' btn-success','')\n\
            button.innerHTML = \"Copy to clipboard\"\n\
            button.disabled = false;\n\
        },2000)\n\
    }\n\
}\n\
\n\
function loadDetails(){\n\
    var sc = document.createElement('object');\n\
    sc.data='DETAILS.TXT'\n\
\n\
    console.log(sc);\n\
    //document.getElementsByTagName(\"head\")[0].appendChild(sc);\n\
    sc.onload = sc.onreadystatechange = function(){\n\
        console.log(sc.contentDocument.document)\n\
    }\n\
}\n\
function loadFlash() {\n\
\n\
    var tabs = document.querySelectorAll('.tabs .tab-links a');\n\
\n\
    for(var i = 0; i < tabs.length; i++){\n\
        tabs[i].onclick = function(e){\n\
            e.preventDefault();\n\
\n\
            var oldTabLink = this.parentNode.parentNode.querySelector(\".active\");\n\
\n\
            oldTabLink.className = oldTabLink.className.replace(\"active\",\"\");\n\
\n\
            this.parentNode.className = this.parentNode.className + ' active'\n\
\n\
            var id = this.href.split(\"#\")[1]\n\
\n\
            var selectedTab = document.getElementById(id);\n\
\n\
            var tabWrap = selectedTab.parentNode;\n\
\n\
            var oldTab = tabWrap.querySelector(\".active\");\n\
\n\
            oldTab.className = oldTab.className.replace(\"active\",\"\");\n\
\n\
            selectedTab.className = selectedTab.className + ' active'\n\
        }\n\
    }\n\
    var sc = document.createElement('script');\n\
    sc.src='FLASHJS.TXT'\n\
    document.getElementsByTagName(\"head\")[0].appendChild(sc);\n\
    sc.onload = sc.onreadystatechange=function(){\n\
        buildIF();\n\
    };\n\
    loadDetails();\n\
}\n\
\n\
function artificialTimeout(){\n\
    var str = \"<table>\";\n\
    str += \"<th><tr><td>Name</td><td>Size</td><td>Actions</td><th></tr>\"\n\
\n\
    for(var i=0;i<10;i++) {\n\
      var file = getFile(i);\n\
      if(file == 0) continue;\n\
      str += \"<tr><td>\"+file.name+\"</td><td>\"+file.size+\"KB</td><td><button type='button' class='btn btn-primary padded' onClick='copyToClipboard(\"+i+\", this)'>Copy to Clipboard</button><a type='button' class='btn btn-default padded'  href='\"+ file.file+\"' download='\"+file.name+\"'>Download</a></td></tr>\";\n\
    }\n\
\n\
    document.getElementById(\"fs\").innerHTML = str+\"</table>\";\n\
}\n\
\n\
function buildIF(){\n\
\n\
    setTimeout(artificialTimeout,1000)\n\
}\n\
</script>\n\
</head>\n\
<body onLoad='loadFlash()'>\n\
    <div class=\"navbar\">\n\
        <center>\n\
            <svg width=\"168\" height=\"54\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n\
                <image x=\"0\" y=\"0\" width=\"168\" height=\"54\" xlink:href=\"data:image/png;base64,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\"/>\n\
            </svg>\n\
            <!--<img src=\"http://lancaster-university.github.io/microbit-docs/img/logo[whiteruntime].png\"></img>-->\n\
        </center>\n\
    </div>\n\
    <div class=\"padded\">\n\
        <div class=\"tabs\">\n\
            <ul class=\"tab-links\">\n\
                <li class=\"active\">\n\
                    <a href=\"#device-info\">Device Information</a>\n\
                </li>\n\
                <li>\n\
                    <a href=\"#file-system\">File System</a>\n\
                </li>\n\
            </ul>\n\
            <div class=\"tab-content\">\n\
                <div id=\"device-info\" class=\"tab active\">\n\
                    <center>\n\
                        <div style=\"display:inline-block;\" class=\"padded\">\n\
                            <svg style=\"float:left;\" width=\"197\" height=\"152\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n\
                                <image x=\"0\" y=\"0\" width=\"197\" height=\"152\" xlink:href=\"data:image/png;base64,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\">\n\
                            </svg>\n\
                            <p  style=\"float:left; text-align:left; font-size:18px;\"  class=\"speech-bubble bubble-align\">\n\
                                Hello, my name is: !\n\
                                <br/>\n\
                                <br/>\n\
                                I'm running DAPLink firmware version: !\n\
                                <br/>\n\
                                <br/>\n\
                                My runtime version is: !\n\
                            </p>\n\
                        </div>\n\
                    </center>\n\
                </div>\n\
                <div id=\"file-system\" class=\"tab\">\n\
                    <div id='fs'>\n\
                        <div class=\"loader\">\n\
                            Loading...\n\
                        </div>\n\
                    </div>\n\
                </div>\n\
            </div>\n\
        </div>\n\
    </div>\n\
    <div class=\"popup-wrapper\" id=\"popup\" onClick=\"this.style.display='none'\">\n\
        <div class=\"popup\">\n\
            <div class=\"info-wrapper\">\n\
                <div class=\"text\">We weren't able to copy it for you :( <br/> Copy this stuff:</div>\n\
                <center>\n\
                    <textarea id=\"clipboard\">\n\
                    </textarea>\n\
                </center>\n\
            </div>\n\
        </div>\n\
    </div>\n\
</body>\n\
</html>\n\
";
