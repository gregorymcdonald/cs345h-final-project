# cs345h-final-project
Team Members: Alex Meed, Brian Zhu, Greg McDonald, Patrick Moore  
Report Link (on Overleaf): https://www.overleaf.com/7044654wzpbhcfhgmmz

Notes:
1. How to Add a Token
	- Open 'parser.y' in an editor
	- Add the token name (e.g. TOKEN_IMPORT) to the end of the token list, indicated by %token.
	- Add a grammar rule of the form to 'parser.y'
		input <TOKEN_NAME>
		{
    		cout << "<TOKEN_NAME>" << endl;
		}
	- Exit editor
	- Enter command 'bison -d parser.y' to recompile the parser (and therefore the token list)
	- Open 'lexer.l' in an editor
	- Add rule recognizing input and returning your new token. e.g.
		(?i:"import") {
    		return TOKEN_IMPORT;
		}
	- Exit editor
	- Enter command 'make lexer'
	- Enter command './lexer.l test.L' to see your new token in action!


