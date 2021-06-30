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
  #print("formula is " ,tokens)
  answer = 0
  tokens.insert(0, {'type': 'PLUS'}) # Insert a dummy '+' token
  index = 1
  while index < len(tokens):
    if tokens[index]['type'] == 'NUMBER':
      if tokens[index - 1]['type'] == 'PLUS' or tokens[index - 1]['type'] == 'MINUS':
        pass
      elif tokens[index - 1]['type'] == 'TIMES':
        tokens[index]['number'] *= tokens[index-2]['number']
        tokens[index-2]['number'] = 0
        tokens[index-1] = {'type':tokens[index-3]['type']}
      elif tokens[index - 1]['type'] == 'DIVISION':
        tokens[index]['number'] = tokens[index-2]['number']/tokens[index]['number']
        tokens[index-2]['number'] = 0
        tokens[index-1] = {'type':tokens[index-3]['type']}
      else:
        #print(tokens[index])
        print('Invalid syntax')
        exit(1)
    index += 1
  
  index = 1
  while index < len(tokens):
    if tokens[index]['type'] == 'NUMBER':
      if tokens[index - 1]['type'] == 'PLUS':
        answer += tokens[index]['number']
      elif tokens[index - 1]['type'] == 'MINUS':
        answer -= tokens[index]['number']
      else:
        print('Invalid syntax')
        exit(1)
    index += 1
  return answer

# extract what's in the parentheses and calculate it with evaluate()
def hundleFormula(tokens):
  number_of_brackets = 0
  for token in tokens:
    if token['type'] == 'LEFTBRACKET':
      number_of_brackets = max(number_of_brackets,token['layer'])
  while number_of_brackets > 0:
    index = 0
    while index < len(tokens):
      if tokens[index]['type'] == 'LEFTBRACKET' and tokens[index]['layer'] == number_of_brackets:
        right_bracket_index = index+1
        while right_bracket_index < len(tokens):
          if tokens[right_bracket_index]['type'] == 'RIGHTBRACKET' and tokens[right_bracket_index]['layer'] == number_of_brackets:
            value_in_brackets = evaluate(tokens[index+1:right_bracket_index])
            tokens[index] = {'type': 'NUMBER', 'number': value_in_brackets}
            for i in range(1,right_bracket_index-index+1):
              if i%2:
                tokens[index+i] = {'type': 'TIMES'}
              else:
                tokens[index+i] = {'type': 'NUMBER', 'number': 1}
            index = right_bracket_index+1
            break
          else : right_bracket_index += 1
      else: index += 1
    number_of_brackets -= 1
  return evaluate(tokens)

# check if the value calculated by hundleFormula() is right
def test(line):
  tokens = tokenize(line)
  actualAnswer = hundleFormula(tokens)
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
