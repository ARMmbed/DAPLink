---
---
function select_daplink_board(board) {
    var board_info;
    for (var i=0; i<_daplink_board_options.data.length; i++) {
        if (_daplink_board_options.data[i].name == board) {
            board_info = _daplink_board_options.data[i];
        }
    }
    if (!board_info) return;

    // Display instructions if not already shown
    if($('#update-instructions').css('display') == 'none'){
        $('#update-instructions').fadeIn();
    }

    // Load instructions to their tabs
    var winvalue = board_info.windows_instructions;
    var linvalue = board_info.linux_instructions;
    var osxvalue = board_info.osx_instructions;
    $("#update-instructions-windows").html(winvalue)
    $("#update-instructions-linux").html(linvalue)
    $("#update-instructions-osx").html(osxvalue)

    //  Output file name
    var fw_name = board_info.fw_name;
    $('#file-name').html('Firmware File: <a href= "{{site.baseurl}}/firmware/'+fw_name +'">' +fw_name +'</a>')

    // Set default tab based on browser
    var os = navigator.platform;

    if (navigator.appVersion.indexOf("Win")!=-1){
        $('.nav-tabs a[href="#update-instructions-windows"]').tab('show');
    }
    if (navigator.appVersion.indexOf("Mac")!=-1){
        $('.nav-tabs a[href="#update-instructions-osx"]').tab('show');
    }
    if (navigator.appVersion.indexOf("X11")!=-1){
        $('.nav-tabs a[href="#update-instructions-linux"]').tab('show');
    }
    if (navigator.appVersion.indexOf("Linux")!=-1){
        $('.nav-tabs a[href="#update-instructions-linux"]').tab('show');
    }
}

var _daplink_board_options = {
	data:[
		{% for thing in site.data.update %} 
		{
			"name": "{{thing.name}}", 
			"code": "{{thing.product_code}}",
			"logoURL": "{{thing.logoURL}}",
			//  assemble name of firmware file
			"fw_name":  {% if thing.fw_name != nil%}
							{%if thing.image_format == nil %}
							"{{site.data.update[0].default.fw_version}}_{{thing.fw_name}}{{site.data.update[0].default.image_format}}",
							{% else %}
							"{{site.data.update[0].default.fw_version}}_{{thing.fw_name}}{{thing.image_format}}",
							{%endif%}
						{%else%}
							"None",
						{%endif%}
			// load instructions if they exist, otherwise load default instructions (at index 0)
			"windows_instructions": {% if thing.instructions.windows == nil %}
								{{site.data.update[0].default.instructions.windows | markdownify | jsonify}},
								{% else %}
								{{thing.instructions.windows | markdownify | jsonify}},
								{% endif %}
			"linux_instructions": {% if thing.instructions.linux == nil %}
								// "test data of awesome"
								{{site.data.update[0].default.instructions.linux | markdownify | jsonify}},
								{% else %}
								{{thing.instructions.linux | markdownify | jsonify}},
								{% endif %}
			"osx_instructions": {% if thing.instructions.osx == nil %}
								// "test data of awesome"
								{{site.data.update[0].default.instructions.osx | markdownify | jsonify}}
								{% else %}
								{{thing.instructions.osx | markdownify | jsonify}}
								{% endif %}
		},
  		{% endfor %}
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