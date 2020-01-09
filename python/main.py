import arrrgh

if __name__ == "__main__":
	print("Initializing arguments...")

	arrrgh.add_option("source");
	arrrgh.add_flag("flag");
	arrrgh.add_flag("brag");
	arrrgh.add_list("tag");

	arrrgh.add_alias('s', "source");
	arrrgh.add_alias('f', "flag");
	arrrgh.add_alias('b', "brag");
	arrrgh.add_alias('t', "tag");

	print("Parsing arguments...")

	arrrgh.parse()

	print("Checking options...")

	for key, value in arrrgh.options.items():
		print("{} -> \"{}\"".format(key, value))

	print("Checking aliases...")

	for key, value in arrrgh.aliases.items():
		print("{} -> {}".format(key, value))

	print("Checking parameters...")

	for it in arrrgh.parameters:
		print(" * {}".format(it))
