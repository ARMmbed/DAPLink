---
---
var options = {
	data:[
		{% for thing in site.data.update %} 
		{
			"name": "{{thing.name}}", 
			"code": "{{thing.product_code}}",
			"logoURL": "{{thing.logoURL}}",
			"instructions": "{{thing.instructions }}"
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
			var value = $("#update-search").getSelectedItemData().instructions;
			$("#update-instructions").html(value)
		}
	},

	theme: "round"

};

$("#update-search").easyAutocomplete(options);