#!/usr/bin/perl 
####################################################################
##           PrimeTime Function for EDA Contest 2016              ##
####################################################################
#Write out the PrimeTime cmd: report_timing -justify
#
#Function
##Reading the timing report and set the path to justify 
##Write out the PrimeTime cmd
##The PrimeTime cmd will exec paralle in PrimeTime
#
#Author
##CIC DSD DTS
##C.L. Huang
#
#Version Info.
##v20101113-1
###Init. veriosn
####################################################################

#./floating_mode_sim.pl ./test pt_to_flaoting.v

####################################
## Usage        
####################################
sub print_usage{

print <<ENDL;   
        
        Usage : floating_mode_sim.pl <DIR> <filename> <verilog_netlist>
               
ENDL
}

####################################
## ARGV Check
####################################
if ($#ARGV == -1) {
    print_usage();
    exit;
}
unless($#ARGV!=3){
    print_usage();
    exit;
}

#################################### 
## VARs 
####################################
#read the first word after floating_mode_sim.pl for directory name
$dir_name = $ARGV[0];
#read the second word after floating_mode_sim.pl for report file name
$rptfile_name = $ARGV[1];
$verilog_name = $ARGV[2];
$verilog_name =~/(case[\d])/s;
#################################### 
## Main                         
####################################
#print "\nStart the perl function.\n";

$start_to_read_verilog = 0;
$start_to_read_module = 0;
$start_to_write_module = 0;

$item_num = 0;
$item_num1 = 0;

$input_port_number = 0;
$define_cnt = 0;
$define_cnt_start = 0;
$define_cnt_stop = 0;


#change directory
#print "\nSTEP: chdir to $dir_name DIR.\n";
#chdir $dir_name || die "Error: Fail to chdir to $dir_name.\n"; 

#remove the existing file with the name floating_mode_testbench?
#print "\nSTEP: Create file system.\n";
if (-e "../tmp/floating_mode_testbench.v") {
	unlink "../tmp/floating_mode_testbench.v";
}

if (-e "../tmp/floating_mode_testbench_v.v") {
	unlink "../tmp/floating_mode_testbench_v.v";
}

#create file floating_mode_testbench
system "/bin/touch ../tmp/floating_mode_testbench.v" || die "Error: Fail to create ../tmp/floating_mode_testbench.v\n";
system "/bin/touch ../tmp/floating_mode_testbench_v.v" || die "Error: Fail to create ../tmp/floating_mode_testbench_v\n";
#system "/usr/bin/perl ../make_dir.pl justify_db" || die "Error: Fail to create justify_db DIR.\n";

#print "\nSTEP: Read the report and write out script.\n";

$justify_cmd = "`timescale 1ns/1ps"."\n"; 
&write_cmd1;
$justify_cmd = "module tb;"."\n"; 
&write_cmd1;
	
&read_verilog;

#for($jj=0;$jj<$input_port_number;$jj++)
#{
	       
#	       $justify_cmd = "input_port$jj = $input_port[$jj] \n";
#	       &write_cmd1;
#}
	       
&read_report;

#################################### 
## Sub
## Read the verilog
####################################
sub read_verilog{
#print "Notice: Read the verilog.\n";
open (REPORTFILE, "<$verilog_name") || die "Error: Can't find the $verilog_name.\n";
@input_port;
while (<REPORTFILE>){
        chomp($_);
        s/^\s+|\t//g;
	#replace tab to space
	#s/\t/ /g;
	#delete the space in the head of line, ^means head
	#s/^\s+//g;
	#print "$_";
	
        if($_=~ s/input //)
	{
	     $start_to_read_verilog = 1;
	     #print "start_to_read_verilog = $start_to_read_verilog\n";
	     $justify_cmd = "reg "; 
	     &write_cmd1;
	}
	if($start_to_read_verilog == 1)
	{
	  
	  $_ =~ s/,/ /g;
	  $_ =~ s/;/ ;/g;
	  @item = split(/\s+/);
	  $item_num = $#item;
	#  print "item # = $#item \n";
	#  print "item0 = $item[0]\n";
	#  print "item1 = $item[1]\n";
	#  print "item2 = $item[2]\n";
	#  print "item3 = $item[3]\n";
	#  print "item4 = $item[4]\n";
	#  print "item5 = $item[5]\n";
	#  print "item6 = $item[6]\n";
	   for($i=0;$i<=$item_num;$i++)
	   {
	    if($item[$i]=~ /;/)
	    {
	     $justify_cmd="tmp_redundant; \n";
	     &write_cmd1;
	    }
	    else
	    {
	    $justify_cmd = "$item[$i], "; 
	    &write_cmd1;
	    #tmp the all input port and count the number of input port for 1'bx
	       $input_port[$input_port_number] = $item[$i];
	       $input_port_number ++;
	       #$justify_cmd = "input_port_number=$input_port_number";
	       #&write_cmd1;
	    }
	   }
	  
	  
	}
	if($_=~/;/)
	{
	     $start_to_read_verilog = 0;
	    # $justify_cmd = "\$start_to_read_verilog = $start_to_read_verilog "; 
	    # &write_cmd1;
	}
	
	
} #end while


close(REPORTFILE);

$justify_cmd = "reg tmp;"."\n"; 
&write_cmd1;

$justify_cmd = "reg [100:0] path;"."\n"; 
&write_cmd1;
		
open (REPORTFILE, "<$verilog_name") || die "Error: Can't find the $verilog_name.\n";


while (<REPORTFILE>){
        chomp($_);
        s/^\s+|\t//g;
	#replace tab to space
	#s/\t/ /g;
	#delete the space in the head of line, ^means head
	#s/^\s+//g;
	
	if($_=~ s/module //)
	{
	  $start_to_read_module = 1;    
	}
	
	$_ =~ s/,/ /g;
	$_ =~ s/;/ ;/g;
	$_=~ s/\(/ dut ( /;
	$_=~ s/\)/ ) /;
	
	#$justify_cmd = "$_ \n"; 
	#       &write_cmd1;
        
	@item1 = split(/\s+/);
	$item_num1 = $#item1;
	 if($start_to_read_module==1)
	 {
	   for($ii=0;$ii<=$item_num1;$ii++)
	   {
	    # $justify_cmd = "now is \"$item1[$ii]\" \n"; 
	    # &write_cmd1;
	     
	     
	     if($start_to_write_module==1)
	     {
	      if($item1[$ii] =~ /\)/)
	      {
	        $start_to_write_module = 0;
		$start_to_read_module = 0;  
	      }
	      elsif($item1[$ii-1] =~ /\(/)
	      {
	      }
	      else
	      {
	       $justify_cmd = ","; 
	       &write_cmd1;
	      }
	     }
	     
	     if($start_to_write_module==1)
	     {
	       $justify_cmd = ".$item1[$ii]($item1[$ii])"; 
	       &write_cmd1;
	       
	       
	       
	     }
	     else
	     {
	       $justify_cmd = "$item1[$ii] "; 
	       &write_cmd1;
	     }
	     
	     
	      
	     if($item1[$ii] =~ /\(/)
	     {
	        $start_to_write_module = 1;
		#$justify_cmd = "start_to_write_module"; 
	        #&write_cmd1;
	     }
	      
	     if($item1[$ii] =~ /;/)
	     {
	        
		$justify_cmd = "\n"; 
	        &write_cmd1;
	     }
	     
	     
	   }
	 }
	
	if($_=~ /\)/)
	{
	  $start_to_read_module = 0;   
	}
	
	
	
} #end while


close(REPORTFILE);


}

#################################### 
## Sub
## Read the report true path sets
####################################
sub read_report{
#print "Notice: Read the report true path sets.\n";

$get_path = 0;
$path_count = 0;
$get_vector = 0;
$cnt_module = 0;
$pt_module = 0;
$path_num = 0;
$justify_cmd_tmp = 0;


                
		
                #$justify_cmd = "`timescale 1ns/1ps"."\n"; 
	        #&write_cmd1;
                #$justify_cmd = "module tb;"."\n"; 
	        #&write_cmd1;
		#$justify_cmd = "reg [1:0]A;"."\n"."reg [1:0]B;"."\n"; 
	        #&write_cmd1;
		#$justify_cmd = "reg tmp;"."\n"; 
	        #&write_cmd1;
		#$justify_cmd = "example dut(.A({A[1],A[0]}),.B({B[1],B[0]}));"."\n"; 
	        #&write_cmd1;
		#$justify_cmd = "initial"."\n"."begin"."\n"."\$sdf_annotate(\"$verilog_name.sdf\",dut);"."\n"."\$dumpfile(\"$verilog_name.vcd\");"."\n"."\$dumpvars;"."\n"."end"."\n"; 
	        $justify_cmd = "initial"."\n"."begin"."\n"."\$dumpfile(\"../output/$1.vcd\");"."\n"."\$dumpvars;"."\n"."end"."\n"; 
		&write_cmd1;
		$justify_cmd = "initial"."\n"."begin"."\n"."path=0;"."\n"; 
	        &write_cmd1;
		#$justify_cmd = "//inital value for missing input from pt report"."\n"."A[0]=0;"."\n"."A[1]=0;"."\n"."B[0]=0;"."\n"."B[1]=0;"."\n"; 
	        #&write_cmd1;
#open(FILEHANDLE, "<filename") , open filename for reading
open (REPORTFILE, "<$rptfile_name") || die "Error: Can't find the $rptfile_name.\n";

while (<REPORTFILE>){
	chomp($_);
        #s/^\s+|\t//g;
	#replace tab to space
	s/\t/ /g;
	#delete the space in the head of line, ^means head
	s/^\s+//g;
	#print "$_\n";
	
	if($_=~/Path/)
	{
	     #\d eq to [0-9]
	     if($_=~/([\d].*[^\}])/s)
	     {
	     $justify_cmd = "\$display(\"path=".$1."\"\);"."\n"; 
	     &write_cmd1;
	     $justify_cmd = "path=".$1.";\n"; 
	     &write_cmd1;
	     $path_num = $1;
	     }
	}
	
	
       # if(!($_=~/\S+/)) {next;}
      #  print "$_\n";
	if ($_=~/\(in\)/) {
	        print "\nNotice: Start fetch path $path_num \n";
		
		######new#####
		@write_tmp = split(/\s+/);
		if($write_tmp[4] eq r){
		$justify_cmd = "tmp=1;"."\n";
	        &write_cmd1;
		}
		if($write_tmp[4] eq f){
		$justify_cmd = "tmp=0;"."\n";
	        &write_cmd1;
		}
		####new######
		
		#$justify_cmd = "module "."test".$cnt_module." ;"."\n"; 
	        #$justify_cmd = "module "."test".$path_num." ;"."\n"; 
		#&write_cmd;
		
		$cnt_module ++;
		
		# if capture (in), get_path=1
		$get_path = 1;
		#$justify_cmd = "\nreport_timing -justify -input_pins \\\n";
	}

	#if ($_=~/data.*arrival.*time/) {
	if ($_=~/Data Required Time/i) {
		if ($get_path == 1) {
			#print "\nNotice: End fetch path.\n";
			$get_path = 0;
			$path_count = $path_count + 1;
		#	print "$justify_cmd\n\n";
			#&write_cmd;
			
			#$justify_cmd = "endmodule"."\n"."\n"; 
	                #&write_cmd;
			#$justify_cmd = "`endcelldefine"."\n"."\n"; 
	                #&write_cmd;
			
		}
	}
	
	if ($_=~/Input Vector/i) {
	        print "\nNotice: Start fetch Input Vector.\n";
		$get_vector = 1;
		
          
	}
        if($get_vector == 1)
	{
	#print "\nget!!!!!!!!!!!\n";
	 if($_=~/\{/)
	   {
	   $justify_cmd = "#100"."\n"; 
	   &write_cmd1;
	   
	   }
	 if($_=~/\}/)
	   {
	   #print "\nnot!!!!!!!!!!!\n";
	   #$justify_cmd = "#100"."\n"; 
	   #&write_cmd1;
	   #$justify_cmd = $justify_cmd_tmp;
	   #&write_cmd1;
	   $justify_cmd = "#0.5"."\n";
	   &write_cmd1;
	   #$pt_module = $cnt_module -1;
	   #$justify_cmd = "test".$pt_module." "."a".$pt_module."()".";"."\n"; 
	   #$justify_cmd = "test".$path_num." "."a".$path_num."()".";"."\n"; 
	   #&write_cmd1;
	   
	   for($call_cnt=$define_cnt_start;$call_cnt<=$define_cnt_stop;$call_cnt++)
	   {
	   $justify_cmd = "`call".$call_cnt."\n";
	   &write_cmd1;
	   }
	   
	   
	   $justify_cmd = "\n"."#10"."\n"; 
	   &write_cmd1;
	   
	   for($jj=0;$jj<$input_port_number;$jj++)
           {
	       
	       $justify_cmd = "$input_port[$jj] = 1'bx;\n";
	       &write_cmd1;
           }

	  # $justify_cmd = "\n"."A[0]=1'bx;"."\n"."A[1]=1'bx;"."\n"."B[0]=1'bx;"."\n"."B[1]=1'bx;"."\n"."\n"; 
	  # &write_cmd1;
	   $get_vector = 0;
	  }
        }
	&process_path;
	&process_vector;
}

close(REPORTFILE);
$justify_cmd = "\$display(\"path=end"."\"\);"."\n"; 
&write_cmd1;
$justify_cmd = "end"."\n"."endmodule"."\n"; 
&write_cmd1;

}


#################################### 
## Sub
## Process the path
####################################
sub process_path{

if ($get_path == 1){
	@configitem = split(/\s+/);
	$confignumber=$#configitem;
	#print "\$confignumber=$confignumber\n";
	#print "$configitem[0] $configitem[4]  ";
	if ($confignumber > 1)
	{
	  if ($configitem[1] =~ "\(in\)") {
	  $define_cnt_start = $define_cnt+1;
	  #print "$configitem[0]";
	  #$justify_cmd = "if(dut.".$configitem[0]."==1'b0)"."\n"."error=1;"."\n"."if(dut.".$configitem[0]."==1'b1)"."\n"."error=1;"."\n"."#1"."\n"."if(dut.".$configitem[0]."==tmp)"."\n"."else"."\n"."error=1"."\n";
		
	  } elsif ($configitem[1] =~  "\(out\)") {
	  $define_cnt_stop = $define_cnt;
                if ($configitem[4] =~  "r") {
                      #  $justify_cmd = $justify_cmd."-rise_to ".$configitem[0]." \\\n";
                } 
	  } else {
	      if ($configitem[0] =~ /.Y/)
	      {
	      $configitem[0] =~ s/.Y/.Y/;
	      $define_cnt++;
              $justify_cmd = "`define call".$define_cnt." if(dut.".$configitem[0]."==1'b0)"."\n"; 
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." begin"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." \$display(\"error $path_num.$configitem[0].Cx0\");"."\n";
  	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." end"."\n";
  	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." if(dut." .$configitem[0]."==1'b1)"."\n"; 
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." begin"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." \$display(\"error $path_num.$configitem[0].Cx1\");"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." end"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." else"."\n"; 
	      &write_cmd;   
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt."   begin"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt."   end"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." #0.5"."\n";
	      &write_cmd;
	      
	      if($configitem[1] =~ /\(NAND2\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=~tmp;"."\n"; 
	      &write_cmd;
	      }
	      if($configitem[1] =~ /\(NOR2\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=~tmp;"."\n"; 
	      &write_cmd;
	      }
	      if($configitem[1] =~ /\(NOT1\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=~tmp;"."\n"; 
	      &write_cmd;
	      }
	      if($configitem[1] =~ /\(BUF1\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=tmp;"."\n"; 
	      &write_cmd;
	      }
	      if($configitem[1] =~ /\(OR2\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=tmp;"."\n"; 
	      &write_cmd;
	      }
	      if($configitem[1] =~ /\(AND2\)/){
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." tmp=tmp;"."\n"; 
	      &write_cmd;
	      }
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." #0.5"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." if(dut.".$configitem[0]."==tmp)"."\n"; 
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." begin"."\n";
	      &write_cmd; 
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." end"."\n";
	      &write_cmd; 
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." else"."\n"; 
	      &write_cmd;   
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt."   begin"."\n";
	      &write_cmd; 
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt." \$display(\"error $path_num.$configitem[0].tmp\");"."\n";
	      &write_cmd;
	      $define_cnt++;
	      $justify_cmd = "`define call".$define_cnt."   end"."\n";
	      &write_cmd;
	      }
	  }
	}
}

}

#################################### 
## Sub
## Process the vector
####################################
sub process_vector{

if ($get_vector == 1){
	@configitem1 = split(/\s+/);
	$confignumber1=$#configitem1;
	#print "\$confignumber1=$confignumber1\n";
      if($confignumber1 > 1)
      {	
	if ($configitem1[1] =~ "=") {
	  if($configitem1[2] =~ "r")
	  {
	   #$justify_cmd_tmp = $configitem1[0].$configitem1[1]."1".";"."\n";
	   $justify_cmd = $configitem1[0].$configitem1[1]."1".";"."\n";
	   &write_cmd1;
	   #$justify_cmd = "tmp=1;"."\n";
	   #&write_cmd1;
	  }
	  elsif($configitem1[2] =~ "f")
	  {
	   #$justify_cmd_tmp = $configitem1[0].$configitem1[1]."0".";"."\n";
	   $justify_cmd = $configitem1[0].$configitem1[1]."0".";"."\n";
	   &write_cmd1;
	   #$justify_cmd = "tmp=0;"."\n";
	   #&write_cmd1;
	  }
	  else
	  {
	   $justify_cmd = $configitem1[0].$configitem1[1].$configitem1[2].";"."\n";
	   &write_cmd1;	
	  }
	}
      } 	
}

}

#################################### 
## Sub
## Write parallel execute cmd
####################################
sub write_cmd{
#print "Notice: Write the cmd.\n";

open (WRITECMDFILE, ">>../tmp/floating_mode_testbench.v") || die "Error: Can't open ../tmp/floating_mode_testbench.v to write.\n";

print WRITECMDFILE $justify_cmd;

close (WRITECMDFILE);

}

sub write_cmd1{
#print "Notice: Write the cmd.\n";

open (WRITECMDFILE, ">>../tmp/floating_mode_testbench_v.v") || die "Error: Can't open ../tmp/floating_mode_testbench_v.v to write.\n";

print WRITECMDFILE $justify_cmd;

close (WRITECMDFILE);

}
