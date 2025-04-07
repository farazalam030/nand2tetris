import JackCompilationEngine as JCE
import JackTokenizer as JT
import sys
import os
import glob


class JackAnalyzer:

	def __init__(self, input_file):
		# assert input_file[-4:] == "jack", "the input file must be Xxx.jack"
		if (input_file[-5:] == ".jack"):
			self.file_name = input_file[:-5]
			self.input_file = input_file
		else:
			print("the input file must be Xxx.jack")
			exit(-1)
		print("analyzing " + self.input_file)

		self.output_file = self.file_name + 'G.xml'

	def analyze(self):
		tokenizer = JT.JackTokenizer(self.input_file)
		tokenizer.tokenize()
		tokenizer.save_tokenfile()
		compileEngine = JCE.JackCompilationEngine(tokenizer.tokens, self.output_file)
		compileEngine.run()
		compileEngine.save_file()


def main():
	file = sys.argv[1]
	if not os.path.exists(file):
		print("the input file does not exist")
		exit(-1)
	if os.path.isdir(file):
		jack_files = glob.glob(file + '/*.jack')
		for jack_file in jack_files:
			JackAnalyzer(jack_file).analyze()
		exit(0)
	JackAnalyzer(file).analyze()

if __name__ == '__main__':
	main()