---
---
function select_daplink_board(board) {
    var board_info;
    for (var i=0; i<_daplink_board_options.data.length; i++) {
        if (_daplink_board_options.data[i].name == board) {
            board_info = _daplink_board_options.data[i];
        };
    };
    if (!board_info) return;

    // Display instructions if not already shown
    if($('#update-instructions').css('display') == 'none'){
        $('#update-instructions').fadeIn();
    }

    // Load instructions to their tabs
    var winvalue = board_info.windows_instructions;
    var linvalue = board_info.linux_instructions;
    var osxvalue = board_info.osx_instructions;
    $("#update-instructions-windows").html(winvalue);
    $("#update-instructions-linux").html(linvalue);
    $("#update-instructions-osx").html(osxvalue);

    //  Output file name
    var fw_name = board_info.fw_name;
    $('#file-name').html('Firmware File: <a href= "{{site.baseurl}}/firmware/'+fw_name +'">' +fw_name +'</a>');

    // Set default tab based on browser
    var os = navigator.platform;

    if (navigator.appVersion.indexOf("Win")!=-1){
        $('.nav-tabs a[href="#update-instructions-windows"]').tab('show');
    };
    if (navigator.appVersion.indexOf("Mac")!=-1){
        $('.nav-tabs a[href="#update-instructions-osx"]').tab('show');
    };
    if (navigator.appVersion.indexOf("X11")!=-1){
        $('.nav-tabs a[href="#update-instructions-linux"]').tab('show');
    };
    if (navigator.appVersion.indexOf("Linux")!=-1){
        $('.nav-tabs a[href="#update-instructions-linux"]').tab('show');
    };
};

var _daplink_board_options = {
    data:[
	{%- for thing in site.data.firmwares -%} 
	{
	    "name": "{{thing.name}}", 
	    "code": "{{thing.product_code}}",
	    "logoURL": "{{thing.logoURL}}",
	    "fw_name": {%- if thing.fw_name != nil -%}
	      {%- if thing.image_format == nil -%}
	        "{{site.data.default.fw_version}}_{{thing.fw_name}}{{site.data.default.image_format}}",
	      {%- else -%}
	        "{{site.data.default.fw_version}}_{{thing.fw_name}}{{thing.image_format}}",
	      {%- endif -%}
	    {%- else -%}
	      "None",
	    {%- endif -%}
	    "windows_instructions": {{site.data.default.instructions[thing.instructions].windows | markdownify | jsonify}},
	    "linux_instructions": {{site.data.default.instructions[thing.instructions].linux | markdownify | jsonify}},
	    "osx_instructions": {{site.data.default.instructions[thing.instructions].osx | markdownify | jsonify}}
	},
  	{%- endfor -%}
    ],

    getValue: "name",

    list: {
	match: {
	    enabled: true
	},
	sort:{
	    enabled:true
	},
	maxNumberOfElements: 8,
	
	showAnimation: {
	    type: "slide",
	    time: 300
	},
	hideAnimation: {
	    type: "slide",
	    time: 300
	},
	onChooseEvent: function(){
	    select_daplink_board($("#update-search").val());
	}
    },
	theme: "round"
};

$("#update-search").easyAutocomplete(_daplink_board_options);

// Check ?board=... and get sht done
var params = [];
var hashes = window.location.href.replace(/#.+/,'').slice(window.location.href.indexOf('?') + 1).split('&');
for(var i = 0; i < hashes.length; i++)
{
    var hash = hashes[i].split('=');
    params[hash[0]] = hash[1];
};

if (typeof params['board'] != 'undefined') {
    $("#update-search").val(params['board']);
    select_daplink_board(params['board']);
};
