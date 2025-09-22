
#pragma once

#include "Client.hpp"
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#define SERVERNAME "server.name"
#define VERSION "v0.1"
#define RPL_WELCOME ":Welcome to " SERVERNAME " " + _clients[_iter].get_user_whole_str()
#define RPL_YOURHOST ":Your host is " SERVERNAME ", running version " VERSION
#define RPL_CREATED                                                      \
	":This server was created " + []()                                   \
	{                                                                    \
		std::stringstream n;                                             \
		n << []()                                                        \
		{                                                                \
			auto t = std::chrono::system_clock::to_time_t(_serverStart); \
			return std::put_time(std::localtime(&t), "%b-%d-%Y");        \
		}();                                                             \
		return n.str();                                                  \
	}()
#define RPL_MYINFO SERVERNAME " " VERSION " o itkl"

#define SEND(fd, mssg) send(fd, mssg, strlen(mssg), 0)

using time_point = std::chrono::time_point<std::chrono::system_clock>;

std::vector<std::string> token_message(std::string line);
std::vector<std::string> split(std::string str, std::string cha);
void sendRplErr(const Client &target, const std::string &prefix, const std::string &command, const std::string &params);
void sendRplErr(const int &target, const std::string &prefix, const std::string &command, const std::string &params);
std::vector<std::string> getBubble(const std::string &input);

constexpr const char* messages [] =
{
	"It is illegal to drive more than two thousand sheep down Hollywood\nBoulevard at one time.\n",
	"The earth is like a tiny grain of sand, only much, much heavier.\n",
	"This is an unauthorized cybernetic announcement.\n",
	"Barometer, n.:\n	An ingenious instrument which indicates what kind of weather we\n	are having.\n		-- Ambrose Bierce, \"The Devil\'s Dictionary\"\n",
	"      /\\	SUN of them wants to use you,\n     \\\\ \\\n  / \\ \\\\ /	SUN of them wants to be used by you,\n / / \\/ / //\\\n \\//\\   \\// /	SUN of them wants to abuse you,\n  / /  /\\  /\n   /  \\\\ \\	SUN of them wants to be abused ...\n     \\ \\\\\n      \\/\n		-- Eurythmics\n",
	"The water was not fit to drink.  To make it palatable, we had to add whiskey.\nBy diligent effort, I learned to like it.\n		-- Winston Churchill\n",
	"Sharks are as tough as those football fans who take their shirts off\nduring games in Chicago in January, only more intelligent.\n		-- Dave Barry, \"Sex and the Single Amoeba: What Every\n		   Teen Should Know\"\n",
	"Over the shoulder supervision is more a need of the manager than the\nprogramming task.\n",
	"Computers don\'t actually think.\n	You just think they think.\n		(We think.)\n",
	"Zymurgy\'s Law of Volunteer Labor:\n	People are always available for work in the past tense.\n",
	"\"Cogito ergo I\'m right and you\'re wrong.\"\n		-- Blair Houghton\n",
	"Home on the Range was originally written in beef-flat.\n",
	"Q:	How do you save a drowning lawyer?\nA:	Throw him a rock.\n",
	"We were so poor we couldn\'t afford a watchdog.  If we heard a noise at night,\nwe\'d bark ourselves.\n		-- Crazy Jimmy\n",
	"... an anecdote from IBM\'s Yorktown Heights Research Center.  When a\nprogrammer used his new computer terminal, all was fine when he was sitting\ndown, but he couldn\'t log in to the system when he was standing up.  That\nbehavior was 100 percent repeatable: he could always log in when sitting and\nnever when standing.\n\nMost of us just sit back and marvel at such a story; how could that terminal\nknow whether the poor guy was sitting or standing?  Good debuggers, though,\nknow that there has to be a reason.  Electrical theories are the easiest to\nhypothesize: was there a loose with under the carpet, or problems with static\nelectricity?  But electrical problems are rarely consistently reproducible.\nAn alert IBMer finally noticed that the problem was in the terminal\'s keyboard:\nthe tops of two keys were switched.  When the programmer was seated he was a\ntouch typist and the problem went unnoticed, but when he stood he was led\nastray by hunting and pecking.\n	-- \"Programming Pearls\" column, by Jon Bentley in CACM February 1985\n",
	"There is an order of things in this universe.\n		-- Apollo, \"Who Mourns for Adonais?\" stardate 3468.1\n",
	"... whether it is better to spend a life not knowing what you want or to\nspend a life knowing exactly what you want and that you will never have it.\n		-- Richard Shelton\n",
	"Forgive him, for he believes that the customs of his tribe are the laws\nof nature!\n		-- G.B. Shaw\n",
	"All true wisdom is found on T-shirts.\n",
	"Ever wonder if taxation without representation might have been cheaper?\n",
	"People don\'t usually make the same mistake twice -- they make it three\ntimes, four time, five times...\n",
	"The only justification for our concepts and systems of concepts is that they\nserve to represent the complex of our experiences; beyond this they have\nno legitimacy.\n		-- Albert Einstein\n",
	"Walk softly and carry a big stick.\n		-- Theodore Roosevelt\n",
	"The first version always gets thrown away.\n",
	"She was good at playing abstract confusion in the same way a midget is\ngood at being short.\n		-- Clive James, on Marilyn Monroe\n",
	"\"The reasonable man adapts himself to the world; the unreasonable one persists\n in trying to adapt the world to himself.  Therefore all progress depends on \n the unreasonable man.\"\n-- George Bernard Shaw\n",
	"Truly great madness can not be achieved without significant intelligence.\n		-- Henrik Tikkanen\n",
	"Troglodytism does not necessarily imply a low cultural level.\n",
	"The more laws and order are made prominent, the more thieves and\nrobbers there will be.\n		-- Lao Tsu\n",
	"Taxes, n.:\n	Of life\'s two certainties, the only one for which you can get\n	an extension.\n",
	"No man in the world has more courage than the man who can stop after\neating one peanut.\n		-- Channing Pollock\n",
	"brain-damaged, generalization of \"Honeywell Brain Damage\" (HBD), a\ntheoretical disease invented to explain certain utter cretinisms in\nMultics, adj:\n	Obviously wrong; cretinous; demented.  There is an implication\n	that the person responsible must have suffered brain damage,\n	because he/she should have known better.  Calling something\n	brain-damaged is bad; it also implies it is unusable.\n",
	"An anthropologist at Tulane has just come back from a field trip to New\nGuinea with reports of a tribe so primitive that they have Tide but not\nnew Tide with lemon-fresh Borax.\n		-- David Letterman\n",
	"	A novice was trying to fix a broken lisp machine by turning the\npower off and on.  Knight, seeing what the student was doing spoke sternly,\n\"You cannot fix a machine by just power-cycling it with no understanding\nof what is going wrong.\"  Knight turned the machine off and on.  The\nmachine worked.\n",
	"Most public domain software is free, at least at first glance.\n",
	"	\"A penny for your thoughts?\"\n	\"A dollar for your death.\"\n		-- The Odd Couple\n",
	"We warn the reader in advance that the proof presented here depends on a\nclever but highly unmotivated trick.\n		-- Howard Anton, \"Elementary Linear Algebra\"\n",
	"X windows:\n	It\'s not how slow you make it.  It\'s how you make it slow.\n	The windowing system preferred by masochists 3 to 1.\n	Built to take on the world... and lose!\n	Don\'t try it \'til you\'ve knocked it.\n	Power tools for Power Fools.\n	Putting new limits on productivity.\n	The closer you look, the cruftier we look.\n	Design by counterexample.\n	A new level of software disintegration.\n	No hardware is safe.\n	Do your time.\n	Rationalization, not realization.\n	Old-world software cruftsmanship at its finest.\n	Gratuitous incompatibility.\n	Your mother.\n	THE user interference management system.\n	You can\'t argue with failure.\n	You haven\'t died \'til you\'ve used it.\n\nThe environment of today... tomorrow!\n	X windows.\n",
	"Actors will happen even in the best-regulated families.\n",
	"Brontosaurus Principle:\n	Organizations can grow faster than their brains can manage them\n	in relation to their environment and to their own physiology:  when\n	this occurs, they are an endangered species.\n		-- Thomas K. Connellan\n",
	"Don\'t go around saying the world owes you a living.  The world owes you\nnothing.  It was here first.\n		-- Mark Twain\n",
	"I\'ll pretend to trust you if you\'ll pretend to trust me.\n",
	"You can learn many things from children.  How much patience you have,\nfor instance.\n		-- Franklin P. Jones\n",
	"Q:	What\'s the difference between a JAP and a baby elephant?\nA:	About 10 pounds.\n\nQ:	How do you make them the same?\nA:	Force feed the elephant.\n",
	"The things that interest people most are usually none of their business.\n",
	"You are lost in the Swamps of Despair.\n",
	"For children with short attention spans: boomerangs that don\'t come back.\n",
	"He who laughs last is probably your boss.\n",
	"Don\'t you wish you had more energy... or less ambition?\n",
	"LOGO for the Dead\n\nLOGO for the Dead lets you continue your computing activities from\n\"The Other Side.\"\n\nThe package includes a unique telecommunications feature which lets you\nturn your TRS-80 into an electronic Ouija board.  Then, using Logo\'s\ngraphics capabilities, you can work with a friend or relative on this\nside of the Great Beyond to write programs.  The software requires that\nyour body be hardwired to an analog-to-digital converter, which is then\ninterfaced to your computer.  A special terminal (very terminal) program\nlets you talk with the users through Deadnet, an EBBS (Ectoplasmic\nBulletin Board System).\n\nLOGO for the Dead is available for 10 percent of your estate\nfrom NecroSoft inc., 6502 Charnelhouse Blvd., Cleveland, OH 44101.\n		-- \'80 Microcomputing\n",
	"A city is a large community where people are lonesome together\n		-- Herbert Prochnow\n",
	"You know, of course, that the Tasmanians, who never committed adultery,\nare now extinct.\n		-- M. Somerset Maugham\n",
	"The heart is wiser than the intellect.\n",
	"Put your Nose to the Grindstone!\n		-- Amalgamated Plastic Surgeons and Toolmakers, Ltd.\n",
	"	The judge fined the jaywalker fifty dollars and told him if he was\ncaught again, he would be thrown in jail.  Fine today, cooler tomorrow.\n",
	"Birds are entangled by their feet and men by their tongues.\n",
	"Tip the world over on its side and everything loose will land in Los Angeles.\n		-- Frank Lloyd Wright\n",
	"job interview, n.:\n	The excruciating process during which personnel officers\n	separate the wheat from the chaff -- then hire the chaff.\n",
	"Progress was all right.  Only it went on too long.\n		-- James Thurber\n",
	"In Greene, New York, it is illegal to eat peanuts and walk backwards on\nthe sidewalks when a concert is on.\n",
	"Every word is like an unnecessary stain on silence and nothingness.\n		-- Beckett\n",
	"\"Today, of course, it is considered very poor taste to use the F-word\nexcept in major motion pictures.\"\n		-- Dave Barry, \"$#$%#^%!^%&@%@!\"\n",
	"If builders built buildings the way programmers wrote programs,\nthen the first woodpecker to come along would destroy civilization.\n",
	"You are transported to a room where you are faced by a wizard who\npoints to you and says, \"Them\'s fighting words!\"  You immediately get\nattacked by all sorts of denizens of the museum: there is a cobra\nchewing on your leg, a troglodyte is bashing your brains out with a\ngold nugget, a crocodile is removing large chunks of flesh from you, a\nrhinoceros is goring you with his horn, a sabre-tooth cat is busy\ntrying to disembowel you, you are being trampled by a large mammoth, a\nvampire is sucking you dry, a Tyranosaurus Rex is sinking his six inch\nlong fangs into various parts of your anatomy, a large bear is\ndismembering your body, a gargoyle is bouncing up and down on your\nhead, a burly troll is tearing you limb from limb, several dire wolves\nare making mince meat out of your torso, and the wizard is about to\ntransport you to the corner of Westwood and Broxton.  Oh dear, you seem\nto have gotten yourself killed, as well.\n\nYou scored 0 out of 250 possible points.\nThat gives you a ranking of junior beginning adventurer.\nTo achieve the next higher rating, you need to score 32 more points.\n",
	"QOTD:\n	I opened Pandora\'s box, let the cat out of the bag and put the\n	ball in their court.\n		-- Hon. J. Hacker (The Ministry of Administrative Affairs)\n",
	"Pandora\'s Rule:\n	Never open a box you didn\'t close.\n",
	"Some people claim that the UNIX learning curve is steep, but at least you\nonly have to climb it once.\n",
	"Gimme Twinkies, gimme wine,\n    Gimme jeans by Calvin Kline ...\nBut if you split those atoms fine,\n    Mama keep \'em off those genes of mine!\n\nGimme zits, take my dough,\n    Gimme arsenic in my jelly roll ...\nCall the devil and sell my soul,\n    But Mama keep dem atoms whole!\n		-- Milo Bloom, \"The Split-Atom Blues,\" in \"Bloom County\"\n",
	"Did you know that the voice tapes easily identify the Russian pilot\nthat shot down the Korean jet?  At one point he definitely states:\n\n	\"Natasha!  First we shoot jet, then we go after moose and squirrel.\"\n\n		-- ihuxw!tommyo\n",
	"Volcanoes have a grandeur that is grim\nAnd earthquakes only terrify the dolts,\nAnd to him who\'s scientific\nThere is nothing that\'s terrific\nIn the pattern of a flight of thunderbolts!\n		-- W.S. Gilbert, \"The Mikado\"\n",
	"	Approaching the gates of the monastery, Hakuin found Ken the Zen\npreaching to a group of disciples.\n	\"Words...\" Ken orated, \"they are but an illusory veil obfuscating\nthe absolute reality of --\"\n	\"Ken!\" Hakuin interrupted. \"Your fly is down!\"\n	Whereupon the Clear Light of Illumination exploded upon Ken, and he\nvaporized.\n	On the way to town, Hakuin was greeted by an itinerant monk imbued\nwith the spirit of the morning.\n	\"Ah,\" the monk sighed, a beatific smile wrinkling across his cheeks,\n\"Thou art That...\"\n	\"Ah,\" Hakuin replied, pointing excitedly, \"And Thou art Fat!\"\n	Whereupon the Clear Light of Illumination exploded upon the monk,\nand he vaporized.\n	Next, the Governor sought the advice of Hakuin, crying: \"As our\nenemies bear down upon us, how shall I, with such heartless and callow\nsoldiers as I am heir to, hope to withstand the impending onslaught?\"\n	\"US?\" snapped Hakuin.\n	Whereupon the Clear Light of Illumination exploded upon the\nGovernor, and he vaporized.\n	Then, a redneck went up to Hakuin and vaporized the old Master with\nhis shotgun.  \"Ha! Beat ya\' to the punchline, ya\' scrawny li\'l geek!\"\n",
	"There is a vast difference between the savage and civilized man, but it\nis never apparent to their wives until after breakfast.\n		-- Helen Rowland\n",
	"God runs electromagnetics by wave theory on Monday, Wednesday, and Friday,\nand the Devil runs them by quantum theory on Tuesday, Thursday, and Saturday.\n		-- William Bragg\n",
	"QOTD:\n	I opened Pandora\'s box, let the cat out of the bag and put the\n	ball in their court.\n		-- Hon. J. Hacker (The Ministry of Administrative Affairs)\n",
	"I\'m not the person your mother warned you about... her imagination isn\'t\nthat good.\n		-- Amy Gorin\n",
	"Gerrold\'s Laws of Infernal Dynamics:\n	(1) An object in motion will always be headed in the wrong direction.\n	(2) An object at rest will always be in the wrong place.\n	(3) The energy required to change either one of these states\n	   will always be more than you wish to expend, but never so\n	   much as to make the task totally impossible.\n",
	"You can\'t play your friends like marks, kid.\n		-- Henry Gondorf, \"The Sting\"\n",
	"If you\'re not part of the solution, you\'re part of the precipitate.\n",
	"FORTUNE\'S GUIDE TO DEALING WITH REAL-LIFE SCIENCE FICTION: #2\nWhat to do...\n    if you get a phone call from Mars:\n	Speak slowly and be sure to enunciate your words properly.  Limit\n	your vocabulary to simple words.  Try to determine if you are\n	speaking to someone in a leadership capacity, or an ordinary citizen.\n\n    if he, she or it doesn\'t speak English?\n	Hang up.  There\'s no sense in trying to learn Martian over the phone.\n	If your Martian really had something important to say to you, he, she\n	or it would have taken the trouble to learn the language before\n	calling.\n\n    if you get a phone call from Jupiter?\n	Explain to your caller, politely but firmly, that being from Jupiter,\n	he, she or it is not \"life as we know it\".  Try to terminate the\n	conversation as soon as possible.  It will not profit you, and the\n	charges may have been reversed.\n",
	"Nostalgia is living life in the past lane.\n",
	"Don\'t worry.  Life\'s too long.\n		-- Vincent Sardi, Jr.\n",
	"And now your toner\'s toney,		Disk blocks aplenty\nAnd your paper near pure white,		Await your laser drawn lines,\nThe smudges on your soul are gone	Your intricate fonts,\nAnd your output\'s clean as light..	Your pictures and signs.\n\nWe\'ve labored with your father,		Your amputative absence\nThe venerable XGP,			Has made the Ten dumb,\nBut his slow artistic hand,		Without you, Dover,\nLacks your clean velocity.		We\'re system untounged-\n\nTheses and papers 			DRAW Plots and TEXage\nAnd code in a queue			Have been biding their time,\nDover, oh Dover,			With LISP code and programs,\nWe\'ve been waiting for you.		And this crufty rhyme.\n\nDover, oh Dover,		Dover, oh Dover, arisen from dead.\nWe welcome you back,		Dover, oh Dover, awoken from bed.\nThough still you may jam,	Dover, oh Dover, welcome back to the Lab.\nYou\'re on the right track.	Dover, oh Dover, we\'ve missed your clean\n					hand...\n",
	"I distrust a man who says when.  If he\'s got to be careful not to drink\ntoo much, it\'s because he\'s not to be trusted when he does.\n		-- Sidney Greenstreet, \"The Maltese Falcon\"\n",
	"	We don\'t claim Interactive EasyFlow is good for anything -- if you\nthink it is, great, but it\'s up to you to decide.  If Interactive EasyFlow\ndoesn\'t work: tough.  If you lose a million because Interactive EasyFlow\nmesses up, it\'s you that\'s out the million, not us.  If you don\'t like this\ndisclaimer: tough.  We reserve the right to do the absolute minimum provided\nby law, up to and including nothing.\n	This is basically the same disclaimer that comes with all software\npackages, but ours is in plain English and theirs is in legalese.\n	We didn\'t really want to include any disclaimer at all, but our\nlawyers insisted.  We tried to ignore them but they threatened us with the\nattack shark at which point we relented.\n		-- Haven Tree Software Limited, \"Interactive EasyFlow\"\n",
	"Pollyanna\'s Educational Constant:\n	The hyperactive child is never absent.\n",
	"And 1.1.81 is officially BugFree(tm), so if you receive any bug-reports\non it, you know they are just evil lies.\"\n(By Linus Torvalds, Linus.Torvalds@cs.helsinki.fi)\n",
	"Dying is one of the few things that can be done as easily lying down.\n		-- Woody Allen\n",
	"Q:	What do you call 15 blondes in a circle?\nA:	A dope ring.\n\nQ:	Why do blondes put their hair in ponytails?\nA:	To cover up the valve stem.\n",
	"Experience is what you get when you were expecting something else.\n",
	"The net is like a vast sea of lutefisk with tiny dinosaur brains embedded\nin it here and there. Any given spoonful will likely have an IQ of 1, but\noccasional spoonfuls may have an IQ more than six times that!\n	-- James \'Kibo\' Parry\n",
	"Everybody likes a kidder, but nobody lends him money.\n		-- Arthur Miller\n",
	"AMAZING BUT TRUE ...\n	If all the salmon caught in Canada in one year were laid end to end\n	across the Sahara Desert, the smell would be absolutely awful.\n",
	"A few hours grace before the madness begins again.\n",
	"\"The Street finds its own uses for technology.\"\n-- William Gibson\n",
	"DYSLEXICS OF THE WORLD, UNTIE!\n",
	"American business long ago gave up on demanding that prospective employees\nbe honest and hardworking.  It has even stopped hoping for employees who are\neducated enough that they can tell the difference between the men\'s room and\nthe women\'s room without having little pictures on the doors.\n		-- Dave Barry, \"Urine Trouble, Mister\"\n",
	"\"We cannot put off living until we are ready.  The most salient characteristic\nof life is its coerciveness; it is always urgent, \"here and now,\" without any\npossible postponement.  Life is fired at us point blank.\"\n-- Ortega y Gasset\n",
	"\"Problem solving under linux has never been the circus that it is under\nAIX.\"\n(By Pete Ehlke in comp.unix.aix)\n",
	"Duty, n:\n	What one expects from others.\n		-- Oscar Wilde\n",
	"Beware of the man who knows the answer before he understands the question.\n"
};