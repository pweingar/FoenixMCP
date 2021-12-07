
import re

# Update version.h with a new build number

with open("version.h") as ver:
    lines = ver.readlines();

with open("version.h", "w") as ver:
    for line in lines:
        match = re.match("\#define\s*VER_BUILD\s*(\d+)", line)
        if match:
            build = int(match.group(1)) + 1
            ver.write('#define VER_BUILD {}\n'.format(build))
        else:
            ver.write(line)
