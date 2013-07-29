Creator	"yFiles"
Version	"2.11"
graph
[
	hierarchic	1
	label	""
	directed	1
	node
	[
		id	0
		label	"Start"
		graphics
		[
			x	543.0
			y	152.0
			w	43.314453125
			h	30.0
			customconfiguration	"ShinyPlateNode3"
			fill	"#00FF00"
			hasOutline	0
		]
		LabelGraphics
		[
			text	"Start"
			fontSize	12
			fontName	"Dialog"
			anchor	"c"
		]
	]
	node
	[
		id	1
		label	"v_ClientNotRunning"
		graphics
		[
			x	543.0
			y	247.0
			w	132.576171875
			h	30.0
			customconfiguration	"ShinyPlateNode3"
			fill	"#FF9900"
			hasOutline	0
		]
		LabelGraphics
		[
			text	"v_ClientNotRunning"
			fontSize	12
			fontName	"Dialog"
			anchor	"c"
		]
	]
	node
	[
		id	2
		label	"v_LoginPrompted"
		graphics
		[
			x	543.0
			y	366.0
			w	118.208984375
			h	30.0
			customconfiguration	"ShinyPlateNode3"
			fill	"#FF9900"
			hasOutline	0
		]
		LabelGraphics
		[
			text	"v_LoginPrompted"
			fontSize	12
			fontName	"Dialog"
			anchor	"c"
		]
	]
	node
	[
		id	3
		label	"v_WhatsNew"
		graphics
		[
			x	538.0
			y	507.5
			w	85.953125
			h	37.0
			customconfiguration	"ShinyPlateNode3"
			fill	"#FF9900"
			hasOutline	0
		]
		LabelGraphics
		[
			text	"v_WhatsNew"
			fontSize	12
			fontName	"Dialog"
			anchor	"c"
		]
	]
	edge
	[
		source	0
		target	1
		label	"e_Init/validLogin=false;rememberMe=false"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
		]
		LabelGraphics
		[
			text	"e_Init/validLogin=false;rememberMe=false"
			fontSize	12
			fontName	"Dialog"
			model	"six_pos"
			position	"tail"
		]
	]
	edge
	[
		source	1
		target	2
		label	"e_StartClient[!rememberMe||!validLogin]"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
		]
		LabelGraphics
		[
			text	"e_StartClient[!rememberMe||!validLogin]"
			fontSize	12
			fontName	"Dialog"
			model	"six_pos"
			position	"tail"
		]
	]
	edge
	[
		source	2
		target	3
		label	"e_ValidPremiumCredentials/validLogin=true;"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
		]
		LabelGraphics
		[
			text	"e_ValidPremiumCredentials/validLogin=true;"
			fontSize	12
			fontName	"Dialog"
			model	"six_pos"
			position	"ttail"
		]
	]
	edge
	[
		source	3
		target	2
		label	"e_Logout"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	538.0
					y	507.5
				]
				point
				[
					x	439.0
					y	493.0
				]
				point
				[
					x	454.0
					y	418.0
				]
				point
				[
					x	522.7
					y	418.0
				]
				point
				[
					x	543.0
					y	366.0
				]
			]
		]
		edgeAnchor
		[
			xTarget	-0.3434595112601813
			yTarget	1.0666666666666667
		]
		LabelGraphics
		[
			text	"e_Logout"
			fontSize	12
			fontName	"Dialog"
			model	"six_pos"
			position	"tail"
		]
	]
	edge
	[
		source	3
		target	1
		label	"e_Exit"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	538.0
					y	507.5
				]
				point
				[
					x	1046.0
					y	485.0
				]
				point
				[
					x	1046.0
					y	260.0
				]
				point
				[
					x	543.0
					y	247.0
				]
			]
		]
		LabelGraphics
		[
			text	"e_Exit"
			fontSize	12
			fontName	"Dialog"
			model	"six_pos"
			position	"tail"
		]
	]
	edge
	[
		source	2
		target	2
		label	"e_ToggleRememberMe/rememberMe=!rememberMe;"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	543.0
					y	366.0
				]
				point
				[
					x	578.5
					y	428.0
				]
				point
				[
					x	685.5
					y	406.0
				]
				point
				[
					x	692.5
					y	366.0
				]
				point
				[
					x	543.0
					y	366.0
				]
			]
		]
		edgeAnchor
		[
			xTarget	0.9996362194563431
		]
		LabelGraphics
		[
			text	"e_ToggleRememberMe/rememberMe=!rememberMe;"
			fontSize	12
			fontName	"Dialog"
			model	"null"
			position	"null"
		]
	]
	edge
	[
		source	2
		target	1
		label	"e_Close"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	543.0
					y	366.0
				]
				point
				[
					x	418.70000000000005
					y	287.0
				]
				point
				[
					x	427.70000000000005
					y	221.0
				]
				point
				[
					x	479.70000000000005
					y	200.0
				]
				point
				[
					x	543.0
					y	247.0
				]
			]
		]
		LabelGraphics
		[
			text	"e_Close"
			fontSize	12
			fontName	"Dialog"
			model	"null"
			position	"null"
		]
	]
	edge
	[
		source	1
		target	3
		label	"e_StartClient[rememberMe&amp;&amp;validLogin]"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	543.0
					y	247.0
				]
				point
				[
					x	476.70000000000005
					y	139.0
				]
				point
				[
					x	350.70000000000005
					y	144.0
				]
				point
				[
					x	105.70000000000005
					y	304.0
				]
				point
				[
					x	65.79907834101387
					y	462.0
				]
				point
				[
					x	467.70000000000005
					y	599.0
				]
				point
				[
					x	538.0
					y	507.5
				]
			]
		]
		LabelGraphics
		[
			text	"e_StartClient[rememberMe&amp;&amp;validLogin]"
			fontSize	12
			fontName	"Dialog"
			model	"null"
			position	"null"
		]
	]
	edge
	[
		source	2
		target	2
		label	"e_InvalidCredentials/validLogin=false;"
		graphics
		[
			type	"quadCurve"
			quadCurveStraightness	0.1
			fill	"#000000"
			targetArrow	"standard"
			Line
			[
				point
				[
					x	543.0
					y	366.0
				]
				point
				[
					x	424.70000000000005
					y	426.0
				]
				point
				[
					x	349.70000000000005
					y	416.0
				]
				point
				[
					x	358.70000000000005
					y	355.0
				]
				point
				[
					x	437.70000000000005
					y	355.0
				]
				point
				[
					x	543.0
					y	366.0
				]
			]
		]
		LabelGraphics
		[
			text	"e_InvalidCredentials/validLogin=false;"
			fontSize	12
			fontName	"Dialog"
			model	"null"
			position	"null"
		]
	]
]
