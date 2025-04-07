class Token:

    def __init__(self, category, value):
        self.value = value
        self.t = category
        self.is_terminal = True
        if value == '<':
            self.form = "<" + category + "> &lt; </" + category + ">"
        elif value == '>':
            self.form = "<" + category + "> &gt; </" + category + ">"
        elif value == '&':
            self.form = "<" + category + "> &amp; </" + category + ">"
        else:
            self.form = "<" + category + "> " + value + " </" + category + ">"


class NonTerminalToken:
    def __init__(self, value, isbegin):
        self.value = value
        self.is_terminal = False

        if isbegin:
            self.form = '<' + value + '>'
        else:
            self.form = '</' + value + '>'


class JackTokenizer:
    """
    filter the input Xxx.jack file into tokens
    """

    def __init__(self, file_name):

        self.tokens = []
        self.codes = []
        self.strings = []
        self.index = 0
        self.keywords = ['class', 'constructor', 'function',
                         'method', 'field', 'static', 'var', 'int', 'char',
                         'boolean', 'void', 'true', 'false', 'null', 'this',
                         'let', 'do', 'if', 'else', 'while', 'return']
        self.keyword_dict = {'class': 'keyword', 'constructor': 'keyword', 'function': 'keyword',  'method': 'keyword', 'field': 'keyword', 'static': 'keyword', 'var': 'keyword', 'int': 'keyword', 'char': 'keyword',  'boolean': 'keyword',
                             'void': 'keyword', 'true': 'keyword', 'false': 'keyword', 'null': 'keyword', 'this': 'keyword',  'let': 'keyword', 'do': 'keyword', 'if': 'keyword', 'else': 'keyword', 'while': 'keyword', 'return': 'keyword'}
        self.symbols = ['{', '}', '(', ')', '[', ']', '.', ',',
                        ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~']
        self.symbols_dict = {'{': 'symbol', '}': 'symbol', '(': 'symbol', ')': 'symbol', '[': 'symbol', ']': 'symbol', '.': 'symbol', ';': 'symbol', '+': 'symbol',
                             '-': 'symbol', '*': 'symbol', '/': 'symbol', '&': 'symbol', '|': 'symbol', '<': 'symbol', '>': 'symbol', '=': 'symbol', '~': 'symbol', ',': 'symbol'}


        if file_name[-5:] == ".jack":
            self.file_name = file_name[:-5]
        else:
            print("the input file must be Xxx.jack")
            exit(-1)

        with open(file_name) as file:
            multi_comments = False
            for line in file:
                current_line = line.strip()
                if multi_comments:
                    if current_line.find('*/') == -1:
                        continue
                    current_line = current_line[current_line.find('*/')+2:]
                    multi_comments = False
                    continue
                if len(current_line) == 0 or current_line.startswith('//'):
                    continue
                if current_line.find('//') != -1:
                    current_line = current_line.split('//')[0]
                if current_line.find('/*') != -1:
                    startMultiComment = current_line.find('/*')
                    endMultiComment = current_line.find('*/')
                    if endMultiComment == -1:
                        multi_comments = True
                        current_line = current_line[:startMultiComment]
                    else:
                        current_line = current_line[:startMultiComment] + current_line[endMultiComment+2:]
                self.codes.append((current_line.strip()))

    def tokenize(self):
        """"""
        self.strings = []
        self.index = 0
        for code in self.codes:
            if code.find('\"') != -1:
                s = code.find('\"')
                e = code.rfind('\"')
                self.strings.append(code[s + 1:e])
                code = code[:s] + code[e:]
            tmp = code.split()
            for x in tmp:
                self.handle_code(x)

    def handle_code(self, code):
        if code[0] in self.symbols:
            self.tokens.append(Token("symbol", code[0]))
            if len(code) == 1:
                return
            self.handle_code(code[1:])
        elif code[0] == '\"':
            self.tokens.append(
                Token("stringConstant", self.strings[self.index]))
            self.index += 1
            if len(code) == 1:
                return
            self.handle_code(code[1:])
        elif code[0].isdecimal():
            integer = code
            for index, chars in enumerate(code[1:], 1):
                if chars.isdecimal() == False:
                    integer = code[:index]
                    break
            intIntger = int(integer)
            if intIntger > 32767 or intIntger < 0:
                print("the integer constant is out of range")
                exit(-1)
            self.tokens.append(Token("integerConstant", integer))
            if len(integer) == len(code):
                return
            self.handle_code(code[len(integer):])
        else:
            iskeyword = False
            for keyword in self.keywords:
                if code.find(keyword) == 0:
                    self.tokens.append(Token("keyword", keyword))
                    if len(keyword) == len(code):
                        return
                    self.handle_code(code[len(keyword):])
                    iskeyword = True
                    break

            if iskeyword:
                return
            identifier = code
            for i, s in enumerate(code):
                if s in self.symbols:
                    identifier = code[:i]
                    break
            self.tokens.append(Token("identifier", identifier))
            if len(identifier) == len(code):
                return
            self.handle_code(code[len(identifier):])

    def save_tokenfile(self):
        with open(self.file_name+'TT.xml', 'w') as file:
            file.write("<tokens>\n")
            for token in self.tokens:
                file.write(token.form+'\n')
            file.write("</tokens>")
