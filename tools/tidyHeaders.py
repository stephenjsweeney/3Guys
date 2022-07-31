import re
import sys

def isHeaderUsed(filename, body):
	base = "/".join(source.split("/")[0:-1])

	with open(base + "/" + filename) as f:
		lines = f.read()

	functions = []

	for line in lines.split("\n"):
		if "(" in line and ")" in line and line.endswith(";"):

			x = re.search("([\w]+)\(", line)
			function = x.group(1)
			functions.append(function)

	found = False

	for function in functions:
		if function in body:
			found = True

	return found

def findIncludes(body):
	ignore = ["cJSON", "cglm"]

	toRemove = []

	for line in body.split("\n"):
		if "#include " in line and "common.h" not in line:
			x = re.search("#include \"(.*)\"", line)

			if x is not None and not any(i in x.group(1) for i in ignore):
				used = isHeaderUsed(x.group(1), body)

				if not used:
					toRemove.append(x.group(1))

	newBody = ""

	for line in body.split("\n"):
		keep = True
		for remove in toRemove:
			if remove in line:
				keep = False

		if keep:
			newBody += line
			newBody += "\n"

	return newBody

source = sys.argv[1]

if source in ["cJSON", "cglm"]:
	exit()

with open(source) as f:
	body = f.read()

newBody = findIncludes(body)

with open(source, "w") as f:
	f.write(newBody)
