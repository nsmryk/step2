def readNumber(line, index):
  number = 0
  while index < len(line) and line[index].isdigit():
    number = number * 10 + int(line[index])
    index += 1
  if index < len(line) and line[index] == '.':
    index += 1
    keta = 0.1
    while index < len(line) and line[index].isdigit():
      number += int(line[index]) * keta
      keta /= 10
      index += 1
  token = {'type': 'NUMBER', 'number': number}
  return token, index

def readPlus(line, index):
  token = {'type': 'PLUS'}
  return token, index + 1

def readMinus(line, index):
  token = {'type': 'MINUS'}
  return token, index + 1

def readTimes(line, index):
  token = {'type': 'TIMES'}
  return token, index + 1

def readDivision(line, index):
  token = {'type': 'DIVISION'}
  return token, index + 1

def readLeftBracket(line, index, layer):
  token = {'type': 'LEFTBRACKET', 'layer': layer}
  return token, index + 1, layer + 1

def readRightBracket(line, index, layer):
  token = {'type': 'RIGHTBRACKET', 'layer': layer - 1}
  return token, index + 1, layer - 1

# read a character and add to tokens
def tokenize(line):
  tokens = []
  index = 0
  layer = 1
  while index < len(line):
    if line[index].isdigit():
      (token, index) = readNumber(line, index)
    elif line[index] == '+':
      (token, index) = readPlus(line, index)
    elif line[index] == '-':
      (token, index) = readMinus(line, index)
    elif line[index] == '*':
      (token, index) = readTimes(line, index)
    elif line[index] == '/':
      (token, index) = readDivision(line, index)
    elif line[index] == '(':
      (token, index, layer) = readLeftBracket(line, index, layer)
    elif line[index] == ')':
      (token, index, layer) = readRightBracket(line, index, layer)
    else:
      print('Invalid character found: ' + line[index])
      exit(1)
    tokens.append(token)
  return tokens

# performs the four arithmetic operations
def evaluate(tokens):
  answer = 0
  new_tokens = []
  tokens.insert(0, {'type': 'PLUS'}) # Insert a dummy '+' token
  index = 1
  while index < len(tokens):
    if tokens[index]['type'] == 'NUMBER':
      if tokens[index - 1]['type'] == 'PLUS' or tokens[index - 1]['type'] == 'MINUS':
        new_tokens.append(tokens[index-1])
        new_tokens.append(tokens[index])
      elif tokens[index - 1]['type'] == 'TIMES':
        new_tokens[-1]['number'] *= tokens[index]['number']
      elif tokens[index - 1]['type'] == 'DIVISION':
        new_tokens[-1]['number'] /= tokens[index]['number']
      else:
        print('Invalid syntax1')
        exit(1)
    if tokens[index]['type'] == 'LEFTBRACKET':
        right_bracket_index = index+1
        while right_bracket_index < len(tokens):
          if tokens[right_bracket_index]['type'] == 'RIGHTBRACKET' and tokens[right_bracket_index]['layer'] == tokens[index]['layer']:
            value_in_brackets = evaluate(tokens[index+1:right_bracket_index])
            break
          else : right_bracket_index += 1
        if tokens[index - 1]['type'] == 'PLUS' or tokens[index - 1]['type'] == 'MINUS':
            new_tokens.append(tokens[index-1])
            new_tokens.append({'type': 'NUMBER', 'number': value_in_brackets})
        elif tokens[index - 1]['type'] == 'TIMES':
            new_tokens[-1]['number'] *= value_in_brackets
        elif tokens[index - 1]['type'] == 'DIVISION':
            new_tokens[-1]['number'] /= value_in_brackets
        else:
            print('Invalid syntax1')
            exit(1)
        
        index = right_bracket_index
    index += 1
  index = 1
  while index < len(new_tokens):
    if new_tokens[index]['type'] == 'NUMBER':
      if new_tokens[index - 1]['type'] == 'PLUS':
        answer += new_tokens[index]['number']
      elif new_tokens[index - 1]['type'] == 'MINUS':
        answer -= new_tokens[index]['number']
      else:
        print('Invalid syntax2')
        exit(1)
    index += 1
  return answer

# check if the value calculated by hundleFormula() is right
def test(line):
  tokens = tokenize(line)
  actualAnswer = evaluate(tokens)
  expectedAnswer = eval(line)
  if abs(actualAnswer - expectedAnswer) < 1e-8:
    print("PASS! (%s = %f)" % (line, expectedAnswer))
  else:
    print("FAIL! (%s should be %f but was %f)" % (line, expectedAnswer, actualAnswer))


# Add more tests to this function :)
def runTest():
  print("==== Test started! ====")
  test("2.0")
  test("1+2")
  test("1+2.0")
  test("2*3.2")
  test("2.1*3.0")
  test("2.1*3.7")
  test("4/2.0")
  test("4.2/2")
  test("2.1/3.7")
  test("2.1/3.7*5.5")
  test("2.1/3.7*5.5-10.0")
  test("5-3.7*5.5")
  test("5-3.7/5.5")
  test("5+3.7*5.5/5+9.8/9.79-2.0*0.1")
  test("(((1)))")
  test("(1/2)")
  test("3*(4-1)")
  test("(1+2)*(2.1/5.6)")
  test("(5/7+2)/(3+2*4.1)")
  test("(1+(2*3.4-(5.6/7.8+9.0)))")
  test("3*(4-1)+((5-1)*3+1)/3")
  print("==== Test finished! ====\n")

runTest()

while True:
  print('> ', end="")
  line = input()
  tokens = tokenize(line)
  answer = evaluate(tokens)
  print("answer = %f\n" % answer)
