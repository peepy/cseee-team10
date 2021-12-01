import os 
import shutil
from pathlib import Path

TARGETS = { "bioreactor-esp32":("LinkMaster", "SubsystemMessage", "MessageHandler"), "bioreactor-arduino":("LinkSlave", "Subsystem", "SubsystemMessage", "MessageHandler"), "bioreactor-messaging":("SubsystemMessage", "MessageHandler")}

srcFolder = "src"
rootPath = str(Path(__file__).parents[0]) 

srcPath = rootPath + "/" + srcFolder
srcContents = os.listdir(srcPath)

print()
print()

for folder, dependencies in TARGETS.items():

	sketchPath = rootPath + "/" + folder
	destPath = sketchPath + "/" + srcFolder

	if isinstance(dependencies, str):
		dependencies = [dependencies]
	else:
		dependencies = list(dependencies)

	print("Copying to '%s'..." %(folder))

	if os.path.isdir(destPath):

		print("> '%s' folder exists '%s'." %(srcFolder, folder))

	else:

		print("> Making folder '%s' in '%s'..." %(srcFolder, folder))
		os.mkdir(destPath)


	shutil.rmtree(destPath)
	print("-> Removed existing contents.")
	print()

	

	for dependency in dependencies:

		print("--> Copying folder '%s' to '%s'..." %(dependency, folder))

		shutil.copytree(srcPath+ "/" + dependency, destPath + "/" + dependency) 
		print("--> Done.")

	print()