.section .data

.global street_crosssection_img
.type street_crosssection_img @object
street_crosssection_img:
	.incbin "../assets/street_crosssection.tim"


.global street_straight_img
.type street_straight_img @object
street_straight_img:
	.incbin "../assets/street_straight.tim"


.global street_t_img
.type street_t_img @object
street_t_img:
	.incbin "../assets/street_t.tim"


.global trees_img
.type trees_img @object
trees_img:
.incbin "../assets/trees.tim"

.global engine_data
.type engine_data @object
engine_data:
.incbin "../test.bin"
