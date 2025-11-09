$fs = 0.1;
$fa = 0.1;

keycap_width = 15.0;
keycap_depth = 14.0;
keycap_height = 2.0;
keycap_r = 3.0;
keycap_skart_height = 0.5;
keycap_skart_thin = 1.0;

backlight_position_north = true; // false means "south"
thin_spot_width = 8.0;
thin_spot_depth = 3.0;
thin_spot_height = 0.8;

bump = true; // bump for home position



rotate([90, 0, 180])
    keycap(bump, ["", "", ""]);

// thin_spot macht so ein cutout hinten mit text
//translate([0, 15,0])
//    thin_spot("", "boot", "");


module keycap(bump, text) {
    difference() {
        translate([0, 0, keycap_height * 0.5])
            rounded_cube(keycap_width, keycap_depth, keycap_height, keycap_r);
        translate([0, backlight_position_north ? 4 : -4, 0])
            thin_spot(text[0], text[1], text[2]);
    }
    translate([0, 0, keycap_skart_height * -0.5])
        difference() {
            rounded_cube(keycap_width, keycap_depth, keycap_skart_height, keycap_r);
            rounded_cube(keycap_width - keycap_skart_thin * 2, keycap_depth -keycap_skart_thin * 2, keycap_skart_height * 4, keycap_r);
        }
    if (bump) {
        bump_for_home_position();
    }
    legs();
}


module rounded_cube(width, depth, height, r) {
    minkowski() {
        cube([width - r * 2, depth - r * 2, height - 0.2], center= true);
        cylinder(r = r, h = 0.2, center= true);
    }
}

module bump_for_home_position() {
    translate([0, keycap_depth * -0.5 + 3.0, keycap_height])
        minkowski() {
            sphere(0.3);
            cube([3.0, 0.001, 0.001], center= true);
        }
}


module thin_spot(ltext, ctext, rtext) {
    r = thin_spot_depth * 0.5;
    translate([0, 0, thin_spot_height * 0.5 - 0.1])
        difference() {
            minkowski() {
                cylinder(h= thin_spot_height, r1= r + 0.5, r2= r, center=true);
                cube([thin_spot_width - r * 2, 0.2, 0.0001], center= true);
            }
            translate([0, 0, thin_spot_height * -1.5])
            linear_extrude(height = thin_spot_height * 3) {
                translate([thin_spot_width * -0.4, thin_spot_depth * -0.4])
                    text(ltext, size= thin_spot_depth * 0.4, halign= "left", valign= "bottom");
                text(ctext, size= thin_spot_depth * 0.8, halign= "center", valign= "center");
                translate([thin_spot_width * 0.4, thin_spot_depth * 0.4])
                    text(rtext, size= thin_spot_depth * 0.4, halign= "right", valign= "top");
            }
    }
}


module legs() {
    choc_distance = 5.7;
    translate([choc_distance * 0.5, 0, 0])
        leg();
    translate([choc_distance * -0.5, 0, 0])
        leg();
}


module leg() {
    foor_width = 1.2 - 0.15;
    foot_depth = 3 - 0.15;
    foot_height = 4;
    cut_width = 0.3;
    cut_height = 1.2;

    translate([foor_width * -0.5, foot_depth * -0.5, foot_height * -1])
        hull() {
            translate ([0, 0, cut_height])
                cube([foor_width, foot_depth, foot_height - cut_height]);
            translate ([cut_width, cut_width, 0])
                cube([foor_width - cut_width * 2, foot_depth - cut_width * 2, cut_height]);
        }
}
