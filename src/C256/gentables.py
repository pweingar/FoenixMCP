#
# Generate the assembly files for both the system call library and the kernel jump tabls
#

sys_call_names = []

# Read in the list of system calls ("sys_")
with open("syscalls.txt", "r") as call_names:
	for line in call_names:
		# Remove comments
		index = line.find("#")
		if index == 0:
			line = ""
		elif index > 0:
			line = line[index - 1:]

		line = line.strip()

		# Skip blank lines
		if len(line) > 0:
			sys_call_names.append(line)

# # Create the system call table, which is used to call into the kernel jump table

# with open("syscalls.s", "w") as f:
# 	for call_name in sys_call_names:
# 		f.write("\t.public sys_{}\n".format(call_name))

# 	f.write("\n")

# 	for call_name in sys_call_names:
# 		f.write("\t.extern sjt_{}\n".format(call_name))
		
# 	f.write("\n\t.section farcode\n\n");

# 	for call_name in sys_call_names:
# 		f.write("sys_{:26}\t.equlab sjt_{}\n".format(call_name + ": ", call_name))

# Create the kernel jump table

with open("jumptable.s", "w") as f:
	for call_name in sys_call_names:
		f.write("\t.public sys_{}\n".format(call_name))

	f.write("\n")

	for call_name in sys_call_names:
		f.write("\t.extern {}\n".format(call_name))
		
	f.write("\n\t.section jumptable\n\n");

	for call_name in sys_call_names:
		f.write("sys_{:26}\tjmp long:{}\n".format(call_name + ": ", call_name))