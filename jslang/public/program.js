//document.writeln($(window).width());

var fileFuncs = {
	"dir": function(item){
		$.getJSON(item.text(), function(result){
			$(".table_list").empty();
			$.each(result, function(i, field){
				console.log('' + i + field);
				$(".table_list").append(`<div title=${field.file}>
					<img src=${field.img} alt=${field.type}/>
					<span>${field.path}</span>
					${field.file}</div>`
				);
			})
		});
	},
	"vdo": function(item){
		$(".table_view").empty()
		.append('<video src="' + item.text() + '" controls="controls">NOT support</video>');
	},
	"ado": function(item){
		$(".table_view").empty()
		.append('<audio src="' + item.text() + '" controls="controls">NOT support</audio>');
	},
	"img": function(item){
		$(".table_view").empty()
		.append('<img src="' + item.text() + '"/>');
	},
	"txt": function(item){
		$(".table_view").empty()
		.load(item.text());
	},
};

function str2rgb(str){
	let rgb = [];
	if(str.length == 7){
		for(let i = 1; i+1 < str.length; i += 2){
			rgb.push(parseInt(str.slice(i, i+2), 16));
		}
	}else if(str.length == 4){
		for(let i = 1; i < str.length; i++){
			rgb.push(parseInt(str[i]+str[i], 16));
		}
	}
		
	//console.log("rgb(" + rgb.join(", ")+ ")");
	return "rgb(" + rgb.join(", ") + ")"
}

$(function(){
	
	$(".table_list_bt").click(function(){
		if($(this).text() == ">"){
			$(this).text("<");
		}else{
			$(this).text(">");
		}
		$(".table_list").animate({width: 'toggle'});
		//$(".table_list").fadeToggle().css("box-shadow", "darkgray 0 0 0.5em 0.1em");
		
	});
	
	$(".table_list div").mouseover(function(){
		if($(this).css("background-color") == str2rgb("#eee"))
			$(this).css("background-color", "#ffffff")
		
		$(this)
		.css("box-shadow", "darkgray 0 0 0.5em 0.1em")
		.css("z-index", "1")
		//.css("font-size", "1em")
	}).mouseout(function(){
		if($(this).css("background-color") == str2rgb("#fff"))
			$(this).css("background-color", "#eeeeee")
		
		$(this).css("box-shadow", "none")
		.css("z-index", "0")
		//.css("font-size", "0.8em")
	}).click(function(){
		if($(this).css("background-color") != str2rgb("#69c")){
			$(".table_list div")
			.css("background-color", "#eeeeee")
			.css("color", "#000")
			.css("box-shadow", "none")
			.css("z-index", "0")
			
			$(this).css("background-color", "#6699cc")
			.css("color", "#fcfcfc")
			.css("box-shadow", "darkgray 0 0 0.5em .2em")
			.css("z-index", "1")
			
			let type = $(this).children("img").attr("alt");
			try{
				fileFuncs[type.toString()]($(this));
			}catch(err){
				console.log(err);
				fileFuncs["txt"]($(this));
			}

		}
		
		$(".table_list").fadeOut("slow");
		$(".table_list_bt").text(">");
	})
	
	//console.log($(".table_list div").text());
})
