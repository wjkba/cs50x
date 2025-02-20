def main():
  text = input("Text: ")

  #// Count the number of letters, words, and sentences in the text
  letters = getLetterCount(text)
  words = getWordCount(text)
  sentences = getSentenceCount(text)

  #// Compute the Coleman-Liau index
  L = (letters/words)*100
  S = (sentences/words)*100
  index =  0.0588 * L - 0.296 * S - 15.8;
  grade = round(index)

  #// Print the grade level
  if grade < 1:
    print("Before Grade 1")
  elif grade >= 16:
    print("Grade 16+")
  else: 
    print(f"Grade {grade}")


def getLetterCount(text):
  letterCount = 0
  for char in text:
    if char.isalpha():
      letterCount+=1
  return letterCount

def getWordCount(text):
  wordCount = 1
  for char in text:
    if char == " ":
      wordCount+=1
  return wordCount

def getSentenceCount(text):
  sentenceCount = 0
  for char in text:
    if char == "." or char == "?" or char == "!":
      sentenceCount+=1
  return sentenceCount



main()