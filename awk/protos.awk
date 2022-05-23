# PURPOSE: Awk version of protos.rb, (less robust, but faster).
# NOTE:    This script can NOT handle function definitions that
#          span multiple lines.
# Samples:
# awk -f protos.awk myfile.c                Overwrites myfile.c and myfile.h.
# awk -v g=true -f protos.awk myfile.c      Overwrites myfile.c and myfile.global.
# awk -v purpose="Purpose of this file." -v header="./include" -f protos.awk myfile.c
#                                           Overwrites myfile.c and include/myfile.h
# Changelog:
# 2022-05-20 Add ability to handle multiple line function declarations. Added automated tests.
# 2021-09-10 Replace reserved name for variable name "include" with "header".
# 201?-??-?? Fix is_this_line_the_end_of_the_function_prototypes().
# 2015-09-16 Fix get_define_name(). Now define is always prefixed with __.
# 2014-11-06 Add detection of header file. Update global list if hfile exists.
# 2014-11-05 Add option purpose which is put in the opening comment of myfile.h.
# 2014-11-05 Add options header to specify an alternate include directory.
# 2014-05-28 Narrow focus of what script does. Now update local prototypes
#            and either generate basic header file in directory as source. Or
#            if you define variable g it will generate a global file, cfile.global.
# 2014-04-02 Fix final move of files. Was testing wrong count for file.
# 	 Variable gcnt is for hfile and lcnt if for cfile.
# 2014-01-28 Fix print_header to generate #define without a path if
#		src is in a different.
#		Add option -v header=include path to specify where to store the header file.
# 2013-09-07 Modify -o to provide alt filename instead of overwriting current filename.
# 2013-09-07 Fix print_header to always generate correct #define __FILE_H__ and filename.
BEGIN {
	if (!header) {
		header=ARGV[1];
		sub(/\/[^/]*$/, "", header);
	}
	lcnt=0;
	gcnt=0;
	mlproto="";

	cfile=ARGV[1];
	cnew=cfile".new~";

	hfile  = get_hfile_name( header, ARGV[1] );
}
{
	if (is_a_function_declaration($0)) {
		$0 = prep_line_format($0);
		if (contains_end_of_function_declaration($0)) {
			sub(/\)/, ");");
			record_prototype($0);
		} else {
			mlproto = $0"\n";  # Multiple line function prototype
		}
	} else if (mlproto != "") {
		if (contains_end_of_function_declaration($0)) {
			mlproto = mlproto $0;
			if (mlproto !~ /;/) {
				sub(/\)/, ");", mlproto);
				record_prototype(mlproto);
			}
			mlproto = "";
		} else {
			mlproto = mlproto $0 "\n";
		}
	}
}
END {
	print "Global count = "gcnt".";
	if (gcnt > 0) {
		print_global_prototypes( hfile );
	}

	if (lcnt > 0)	{
		print_local_prototypes(cnew);
		system ("mv "cnew" "cfile);
	}
}
function is_a_function_declaration(line) {
	return line ~ /^[a-zA-Z].*\(.*[^;]$/
}
function prep_line_format( prototype ) {
	prototype = remove_consecutive_spaces(prototype);
	prototype = adjust_spacing_around_opening_parenthesis(prototype);
	prototype = no_spacing_around_closing_parenthesis(prototype);
	prototype = remove_curly_brace_from_end_of_line(prototype);
	return prototype;
}
function contains_end_of_function_declaration(line) {
	return line ~ /)/;
}
function record_prototype(prototype) {
	if (prototype ~ /^static/) {
		local[lcnt++]=prototype;
	} else {
		global[gcnt++]=prototype;
	}
}
function get_define_name( fn ) {
	sub(/\.h+$/, "_H__", fn);
	sub(/^.*\//, "", fn);
	sub(/^/, "__", fn);
	fn = toupper( fn );
	return fn;
}
function get_hfile_name( header, fn ) {
	sub(/.*\//, header"/", fn);
	sub(/\.c+$/, ".h", fn);

	return fn;
}
function get_timestamp() {
	cmd = "date +%Y-%m-%d"	# get date stamp
	cmd | getline mydate;
	close (cmd);
	return mydate;
}
function print_global_prototypes( hfile ) {

	define = get_define_name( hfile );

	if (file_exists( hfile )) {
		update_globals( hfile );
	} else {
		print_header( define, hfile );
	}
	print "#endif /* "define" */" >> hfile;
}
function print_header( define, fout ) {
	filename = fout;
	sub(/^.*\//, "", filename);

	mydate = get_timestamp();
	print "/* "filename"\t\t\t\tDATE: "mydate >fout;
	if (purpose) {
		print " * PURPOSE: "purpose > fout;
	}
	print " */" > fout;
	print "#ifndef "define > fout;
	print "#define "define"\n" > fout;
	print_globals( fout );
}
function print_globals( fout ) {
	print "/* Global count = " gcnt " */" > fout;
	for (x=0; x<gcnt; x++) {
		print global[x] > fout;
	}
}
function is_this_line_the_begining_of_function_prototypes(line) {
	return (line ~ / * prototypes/);
}
function is_this_line_the_end_of_the_function_prototypes(line) {
	if (length(line) == 0)
		return 1;
	else if (line ~ /#endif/)
		return 1;
	else
		return 0;
}
function now_print_local_protos(fout) {
	print " * prototypes\n */" > fout;
	for (i=0; i<lcnt; i++) {
		print local[i] > fout;
	}
	print "" > fout;
}
function print_local_prototypes(new) {
	skip=0;
	close (cfile);

	while ((getline line < cfile) > 0) {
		if (is_this_line_the_begining_of_function_prototypes(line)) {
			skip=1;
		}
		if (skip) {
			if (is_this_line_the_end_of_the_function_prototypes(line)) {
				skip=0;
				now_print_local_protos(new);
			}
		} else {
			print line > new;
		}
	}
	close (cfile);
}
function update_globals( fout ) {
	skip=0;
	close (hfile);

	while ((getline line < hfile) > 0) {
		if (line ~ /\/\* [gG]lobal count =/) {
			skip=1;
		}
		if (skip) {
			if (is_this_line_the_end_of_the_function_prototypes(line)) {
				skip=0;
				print_globals( fout );
				break;
			}
		} else {
			print line > fout;
		}
	}
	close (hfile);
}
function file_exists( file ) {
	if ( (getline line < file) > 0 ) {
		close(file);
		return 1;
	} else {
		return 0;
	}
}
function remove_consecutive_spaces( prototype ) {
	gsub(/ +/, " ", prototype);
	return prototype;
}
function adjust_spacing_around_opening_parenthesis(prototype) {
	sub(/ ?[(] ?/, " (", prototype);
	return prototype;
}
function no_spacing_around_closing_parenthesis(prototype) {
	sub(/ ?[)] ?/, ")", prototype);
	return prototype;
}
function remove_curly_brace_from_end_of_line(prototype) {
	sub(/ ?{ ?$/, "", prototype);
	return prototype;
}
