'''
"Text Sentiment Analyzer"
Author: 44N
'''
#include libraries
import tkinter as tk #for Graphical User-Interface
import matplotlib.pyplot as plt #for plotting the Sentimental Analysis Graph
from textblob import TextBlob #for performing the text's sentimental analysis

#text to be analyzed
text = ""

#display message
msg = "Enter the text to be analyzed"

#result of the analysis
res = ""

#x-axis points are the number of the sentence
x = []

#y-axis points are the scale of positivity or negativity of a sentence (in percentage)
y = []


class Application(tk.Frame):

	def __init__(self, master=None):
		tk.Frame.__init__(self, master, bg="#121212")
		self.pack()
		self.createWidgets()

	def createWidgets(self):
		#introductory message
		self.msg = tk.Message(self, text=msg, width=200, bg='#121212', fg='#ccc')
		self.msg.pack(side="top")

		#text box for input
		self.txt = tk.Text(self, width=100, height=32, padx=5, pady=5, bg='#212121', fg='#eee', relief='flat', insertbackground='#fff')
		self.txt.pack(side="top")

		#result text for displaying analysis results(initially empty)
		self.res = tk.Message(self, text=res, width=400, padx=10, pady=10, bg='#121212', fg='#eee', relief='flat')
		self.res.pack(side="top")

		#button to call submitText() which performs the analysis
		self.clr = tk.Button(self, text='Analyze', command=self.submitText, bg='#101010', fg='#ccc', relief='groove', padx=394)
		self.clr.pack(side="top")

		#clear text
		self.submit = tk.Button(self, text='Clear', command=self.clrTxt, bg='#101010', fg='#ccc', relief='groove', padx=193)
		self.submit.pack(side="left")

		#quit window
		self.quit = tk.Button(self, text='Quit', command=self.master.destroy, bg='#101010', fg='#ccc', relief='groove', padx=193)
		self.quit.pack(side="right")

	#function to clear text box
	def clrTxt(self):
		self.txt.delete('1.0', 'end')
		self.txt.update()

	#performs core analysis of the text & plots the output using pyPlot
	def submitText(self):
		#initalize empty data points sets
		x = []
		y = []

		text = self.txt.get("1.0","end") #take input into a string
		blob = TextBlob(text) #create blob object to use for analysis and feed it the text input by user, which automatically performs the Sentiment Analysis

		#initalizes the different numbers
		numofsentences = positive = negative = neutral = 0

		#if the sentence is positive subjectively then its polarity would be greater than 0 else if its 0 then it's neutral otherwise it is negative subjectively for the topic
		for _ in blob.sentences:
			if (_.sentiment.polarity >= 0):
				y.append(_.sentiment.polarity * 100) #it converts to percentage
				if (_.sentiment.polarity == 0):
					neutral += 1
				else:
					positive += 1
			else:
				y.append(_.sentiment.polarity * 100) #it converts to percentage
				negative += 1
			numofsentences += 1

		for i in range(1, numofsentences+1):
			x.append(i)

		#setup the pyPlot graph
		fig = plt.figure()
		ax = plt.subplot()
		ax.set_ylim(-100, 100)

		ax.plot(x, y, '-') #plots data to graph
		ax.set_title('Text Sentimental Analysis Graph')
		fig.show() #displays the resulting graph
		res = 'Result:\nThe text contains {} sentences.\nNumber of positive sentences: {}\nNumber of neutral sentences: {}\nNumber of negative sentences: {}'.format(numofsentences, positive, neutral, negative)
		self.res.config(text=res, justify='left')


def main():
	app = Application()
	app.master.title('Text Sentiment Analyzer')
	app.mainloop()


if (__name__ == '__main__'):
	main()
