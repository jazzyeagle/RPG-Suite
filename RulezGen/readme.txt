The basics of using RulezGen
----------------------------

Please note at this time that I consider the program a semi-functional
prototype.  I do not believe enough is coded into RulezGen to create a
character in ANY game system yet.  I plan on working based off of FFRPG (an
RPG I found online with rules that have a similar complexity to D&D) to start
identifying the short-comings and building those into RulezGen to make it more
comprehensive, once I'm done with a semi-functional prototype for CharGen.
The only reason I plan to do it that way is because I want people interested in
helping develop to be able to see/realize my vision so they can make a better
decision as to whether or not they want to help.

As the work continues on the Suite, the original assumption is going to be that
the user is doing everything exactly as intended.  This will be fixed as devel-
opment continues in that such an assumption will eventually go away and fail-
safes will be put in place to protect the app from wrong things happening.

All that being said, what is correct needs to be clearly documented in order for
other developers who wish to help contribute to the project to know what is
correct.  This text is a quick rundown of the anticipated user workflow to
help facilitate the development and testing.


1) When the program opens, select File -> New.  This will bring up the ability
to identify if the user is creating a rulebook for a new game (meaning one in
which they do not already have a core rulebook for) or if they're creating a
new rulebook for an existing game (meaning one they DO already have a core
rulebook for).  <New function is not yet written.  The program currently
defaults to opening up a blank core rulebook for a new game.  When 'New'
functions are written, add on to this>

2) Go through the properties menu and review all the properties for each
category.  There are default properties that will be needed for CharGen, but
there is the option to add any additional property fields to any category.
When the actions are implemented, any additional properties could be used to
provide additional functionalitiy in either CharGen or during the game in
the RPGClient.  Because of the generalities, most properties can be reviewed in
any order, but the list from top to bottom mirrors the variables for ease of
switching between the two menus.  The only exception to this is the Item
properties:  This requires that the Item Type properties be completed first, as
the user can input separate properties for each Item Type.

3) Once the properties are all set up, it's time to review the variable tables.
This list should be worked from top to bottom.  For instance, the Skill tables
require that Attributes already be completed, and Modifiers require both Skills
and Attributes in order to be set up properly, and all of them (plus Items)
need currency to be completed first.

4) Once the Variable Tables are complete, proceed to the Pages menu.  This allows
you to add Pages (Images) of the Character sheet as a tab.  Once the Pages are
loaded, click on the textbox or checkbox icons to begin drawing text boxes or
check boxes (respectively) on the character sheet.  Right click on the new box
and select from the options which property/variable you want to link that box
to.  These will become the user inputs for CharGen (character generator).

5) Save/Save As.
