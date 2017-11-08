#import nltk
import re

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # Open and load positive words
        with open(positives) as positives:
        	for line in positives:
        		if line.startswith(";"):
        			#pass
        			continue
        		else:
        			pos_words = positives.read().split()

        self.positives = pos_words


        # Open and load negative words
        with open(negatives) as negatives:
        	for line in negatives:
        		if line.startswith(";"):
        			#pass
        			continue
        		else:
        			pos_words = negatives.read().split()

        self.negatives = pos_words
    

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # Convert to lower case and split words into list
        text = text.lower().split()

        # Strip off extraneous characters
        pattern = r"(\w+)[+-]*(\w+)*"
        for i in range(len(text)):
        	match = re.search(pattern, text[i])
        	if match:
        		text[i] = match.group()


        for word in text:
        	score = 0
        	if word in self.positives:
        		score += 1
        	elif word in self.negatives:
        		score -= 1
        	else:
        		continue

        return score
