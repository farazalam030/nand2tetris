import JackCompilationEngine as JCE
import JackTokenizer as JT
import sys
class JackAnalyzer:
	"""
	JackAnalyzer is the first part of JackCompiler

	Its work contains three parts as follows:
	1. create a JackTokenizer according to the input Xxx.jack file
	2. use JackCompilationEngine to translate the tokens into grammatical tree
	3. output the Xxx.xml file
	"""
	def __init__(self, input_file):
		assert input_file[-4:] == "jack", "the input file must be Xxx.jack"
		self.input_file = input_file
		self.file_name = input_file[:-5]
		self.output_file = self.file_name + '1.xml'

	def analyze(self):
		tokenizer = JT.JackTokenizer(self.input_file)
		tokenizer.tokenize()
		tokenizer.save_tokenfile()
		compileEngine = JCE.JackCompilationEngine(tokenizer.tokens, self.output_file)
		compileEngine.run()
		compileEngine.save_file()


def main():
	file = sys.argv[1]
	JackAnalyzer(file).analyze()

if __name__ == '__main__':
	main()