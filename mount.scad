$fa=2.1;
$fs=0.5;
mdist = 56;
mhole = 4.5;

bd = 70;
bh = 4;

hex = 28;
hex_h = 10;

//basemount();
//translate([0,0,0]) rotate([180,0,0]) logitech_mount();

//translate([0,0,0]) rotate([180,0,0]) f1_mount();

translate([0,0,-30]) f1_mount();
//logitech_mount();

module basemount() {
    translate([0,0,-bh-5]) difference() {
       union() {
           cylinder(d=bd, h=bh,$fn=6);
           
           cylinder(d=hex, h=bh+hex_h, $fn=6);
       }
       rotate([0,0,60*1]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       rotate([0,0,60*2]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       rotate([0,0,60*3]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       rotate([0,0,60*4]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       rotate([0,0,60*5]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       rotate([0,0,60*6]) translate([mdist/2, 0,-0.5]) cylinder(d=mhole, h=bh+1);
       
       %color("blue")rotate([0,0,60*6]) translate([mdist/2, 0,bh]) cylinder(d=8, h=5);
       
       
       rotate([0,0,60*1]) translate([0,70/2,bh+5]) rotate([90,0,0])cylinder(d=3.85, h=70);
       rotate([0,0,60*2]) translate([0,70/2,bh+5]) rotate([90,0,0])cylinder(d=3.85, h=70);
       
   }
    
}


module logitech_mount() {
    extrahole_d = 15.2;
    extrahole_h = 2;
    M4 = 2.85;
    bh = 4;
    
    translate([0,0,-6]) difference() {
       union() {
           cylinder(d=bd, h=bh,$fn=6);
           
           cylinder(d=hex+6, h=hex_h+0.5, $fn=6);
           for ( i = [0 : 1 : 6] ){
               rotate([0,0,60*i]) translate([mdist/2, 0,bh]) cylinder(d=M4+3, h=2);
               
           }
       }
       for ( i = [0 : 1 : 6] ){
           rotate([0,0,60*i]) translate([mdist/2, 0,-0.5]) cylinder(d=M4, h=bh+100);
           rotate([0,0,60*i]) translate([mdist/2, 0,-0.5]) ring();
       }

       rotate([0,0,60*1]) translate([0,70/2,6]) rotate([90,0,0])cylinder(d=4, h=70);
       rotate([0,0,60*2]) translate([0,70/2,6]) rotate([90,0,0])cylinder(d=4, h=70);
       
       rotate([0,0,60*1]) translate([0,-hex/2-0.7,6]) rotate([90,0,0])cylinder(d=8, h=70);
       rotate([0,0,60*5]) translate([0,-hex/2-0.7,6]) rotate([90,0,0])cylinder(d=8, h=70);
       
       translate([0,0,-1]) cylinder(d=hex+1, h=bh+hex_h+1, $fn=6);
       
       translate([0,20,0])cube([3,20,bh]);
       rotate([0,0,180]) translate([0,20,0])cube([3,20,bh]);
       
   }
    
    
}

module ring() {
    extrahole_d = 15.2;
    extrahole_h = 2;
    difference() {
       union() {
            cylinder(d=extrahole_d, h=extrahole_h);
           
       }
       cylinder(d=9, h=extrahole_h);
   }
}


module f1_mount() {
    extrahole_d = 15.2;
    extrahole_h = 2;
    M4 = 2.85;
    bh = 4;
    hex_mount = 34-5;
    
    f1_dist = 28;
    translate([0,0,-6]) difference() {
       union() {
          // cylinder(d=bd, h=bh,$fn=6);
           translate([-36/2, -36/2, 0]) cube([36,36,34]);
           
           cylinder(d=hex+6, h=hex_h+0.5, $fn=6);
           
       }
       

       rotate([0,0,60*1]) translate([0,70/2,hex_mount]) rotate([90,0,0])cylinder(d=4, h=70);
       rotate([0,0,60*2]) translate([0,70/2,hex_mount]) rotate([90,0,0])cylinder(d=4, h=70);
       
       rotate([0,0,60*1]) translate([0,-hex/2-0.7,hex_mount]) rotate([90,0,0])cylinder(d=8, h=70);
       rotate([0,0,60*5]) translate([0,-hex/2-0.7,hex_mount]) rotate([90,0,0])cylinder(d=8, h=70);
       
       translate([0,0,-1]) cylinder(d=hex+1, h=40, $fn=6);
       
       translate([-f1_dist/2, -f1_dist/2,0]) cylinder(d=3, h = 40);
       translate([f1_dist/2, -f1_dist/2,0]) cylinder(d=3, h = 40);
       translate([-f1_dist/2, f1_dist/2,0]) cylinder(d=3, h = 40);
       translate([f1_dist/2, f1_dist/2,0]) cylinder(d=3, h = 40);
       
   }
    
    
}