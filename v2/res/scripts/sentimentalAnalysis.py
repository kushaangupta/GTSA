import matplotlib.pyplot as plt
from textblob import TextBlob
import sys
#initalize empty data points sets
x = []
y = []
f = open(' '.join(sys.argv[1 : len(sys.argv)]), 'r')
text = ''''''
tmp = ''
flag = True
while flag:
	tmp = f.readline()
	if (tmp == ''):
		flag = False
		continue
	text += tmp #take input into a string
blob = TextBlob(text) #create blob object to use for analysis and feed it the text input by user, which automatically performs the Sentiment Analysis

#initalizes the different numbers
numofsentences = positive = negative = neutral = 0

#if the sentence is positive subjectively then its polarity would be greater than 0 else if its 0 then it's neutral otherwise it is negative subjectively for the topic
for _ in blob.sentences:
	if (_.sentiment.polarity >= 0):
		y.append(str(_.sentiment.polarity * 100)) #it converts to percentage
		if (_.sentiment.polarity == 0):
			neutral += 1
		else:
			positive += 1
	else:
		y.append(str(_.sentiment.polarity * 100)) #it converts to percentage
		negative += 1
	numofsentences += 1

for i in range(1, numofsentences+1):
	x.append(i)
'''
#setup the pyPlot graph
fig = plt.figure()
ax = plt.subplot()
ax.set_ylim(-100, 100)

ax.plot(x, y, '-') #plots data to graph
ax.set_title('Text Sentimental Analysis Graph')
#fig.show() #displays the resulting graph
plt.show(block=True) #windows fix[alternate]

res = 'Result:\nThe text contains {} sentences.\nNumber of positive sentences: {}\nNumber of neutral sentences: {}\nNumber of negative sentences: {}'.format(numofsentences, positive, neutral, negative)
print(res)'''

print('\n'.join(y)) #converted y to list of strings for using join