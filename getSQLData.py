import pymysql.cursors
'''
	To Use:
	type in console: python getSQLData.py
	You'll then be prompted to enter a game number to get information about it
	Note: you should be using python 2.7 for this script to work.
'''
# Connect to the database
connection = pymysql.connect(host='localhost',
                             user='root',
                             db='gameEngine',
                             charset='utf8mb4',
                             cursorclass=pymysql.cursors.DictCursor)
gameID=''
print("input a game ID:")
while True:
	try:
		gameID= input()
		if not isinstance(gameID, (int,long)):
			raise NameError
	except NameError:
		print("sorry that is not an integer try again:")
		continue
	else:
		print "gameID =", gameID
		break
gameCreated = None
gameFinished = None
p1ID = None
p2ID = None
c1ID = None
c2ID = None
p1c1ChatID=None
p2c2ChatID=None
p1Name= None
p2Name= None
c1Name = None
c2Name = None
events=[]		
try:
	#get game
	with connection.cursor() as cursor:
   		sql = "SELECT * FROM `games` WHERE id='%s'"
   		cursor.execute(sql % gameID)
   		#print sql %gameID
   		data= cursor.fetchone()
   		gameCreated=data['created']
   		gameFinished=data['finished']
   		p1ID=data['player1ID']
   		p2ID=data['player2ID']
   		c1ID=data['coach1ID']
   		c2ID=data['coach2ID']
   		events.append((gameCreated,"Game %s created." %gameID))
   		events.append((gameFinished, "Game %s finished." % gameID))
   	#get names from users
   	with connection.cursor() as cursor:
   		sql ="SELECT `id`, `name` FROM `users` WHERE id IN (%s, %s, %s, %s)"
   		#print sql %(p1ID, p2ID, c1ID, c2ID)
   		cursor.execute(sql %(p1ID, p2ID, c1ID, c2ID))
   		for row in cursor:
   			if row['id']==p1ID:
   				p1Name= row['name']
   			elif row['id']==p2ID:
   				p2Name= row['name']
   			elif row['id']==c1ID:
   				c1Name = row['name']
   			elif row['id']==c2ID:
   				c2Name=row['name']
   		#print p1Name, p2Name, c1Name, c2Name
   	#get choice times
   	with connection.cursor() as cursor:
   		sql="SELECT * FROM `rounds` WHERE gameID='%s'"
   		cursor.execute(sql % gameID)
   		#print(data)
   		for row in cursor:
   			events.append((row['p1ChoiceTime'], "%s selected option %s" %(p1Name, row['player2choice'])))
   			events.append((row['p2ChoiceTime'], "%s selected option %s" %(p2Name, row['player2choice'])))

   	with connection.cursor() as cursor:
   		sql="SELECT * FROM `chatID` WHERE gameID='%s'"
   		cursor.execute(sql % gameID)
   		for row in cursor:
	   		if row['chatType']=='P/C':
				if row['user1']==p1ID:
					p1c1ChatID=row['id']
				if row['user1']==p2ID:
					p2c2ChatID=row['id']
   	with connection.cursor() as cursor:
   		sql="SELECT * FROM `chats` WHERE chatID IN (%s, %s)"

   		cursor.execute(sql % (p1c1ChatID,p2c2ChatID))
   		for row in cursor:
   			if row['chatID']==p1c1ChatID:
   				if row['userID']==p1ID:
   					events.append((row['created'],'%s to %s: %s' %(p1Name, c1Name, row['message'])))
   				elif row['userID']==c1ID:
   					events.append((row['created'],'%s to %s: %s' %(c1Name, p1Name, row['message'])))
   			elif row['chatID']==p2c2ChatID:
   				if row['userID']==p2ID:
   					events.append((row['created'],'%s to %s: %s' %(p2Name, c2Name, row['message'])))
   				elif row['userID']==c2ID:
   					events.append((row['created'],'%s to %s: %s' %(c2Name, p2Name, row['message'])))

finally:
	connection.close()
events.sort(key=lambda tup: tup[0])
file= open("game.txt","w")
for row in events:
	file.write(str(row[0]-gameCreated)+' --- '+row[1]+'\n')
file.close()
print "See results in game.txt"