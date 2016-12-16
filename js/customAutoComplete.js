---
---
var options = {
	data:[
		{% for thing in site.data.update %} 
		{
			"name": "{{thing.name}}", 
			"code": "{{thing.product_code}}",
			"logoURL": "{{thing.logoURL}}",
			"windows_instructions": {{thing.instructions.windows | markdownify | jsonify}},
			"linux_instructions": {{thing.instructions.linux | markdownify | jsonify}},
			"osx_instructions": {% if thing.instructions.osx == nil %}
								// "test data of awesome"
								{{site.data.update[default].instructions.osx | markdownify | jsonify}}
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
			// Display instructions if not already shown
			if($('#update-instructions').css('display') == 'none'){
				$('#update-instructions').fadeIn();
			}
			// Load instructions to their tabs
			var winvalue = $("#update-search").getSelectedItemData().windows_instructions;
			var linvalue = $("#update-search").getSelectedItemData().linux_instructions;
			var osxvalue = $("#update-search").getSelectedItemData().osx_instructions;
			$("#update-instructions-windows").html(winvalue)
			$("#update-instructions-linux").html(linvalue)
			$("#update-instructions-osx").html(osxvalue)

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
	},

	theme: "round"

};

$("#update-search").easyAutocomplete(options);