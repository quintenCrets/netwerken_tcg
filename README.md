# Tcg service

## Base game explenation

### Trading Card Game

A game that is based around collecting and fighting other cards.

Players will be able to collect mana, this mana can be used to get cards and play them. Some cards will make the player receive more mana or upgrade the amount of mana they can have at maximum. This while they can be attacked by other players if they wish to engage in battle.

### Cards

There are three main type of cards, tools, spells and monsters. Tools can be used to upgrade certain values like maximum mana or the amount of monsters someone can have in play, while spells are instant effects that can do various things. Then there are monsters wich need mana to be summoned and can be used for defence or attack.

### Life points

Players need to be carefull though, as they can be attacked by other players at random. If a player loses all of there life points, they will lose some of the cards they collected, wich can lead to defestating chains if they lose the card witch upgraded the amount of mana they got every action.

## Service calls over benthernet

### Benthernet token

To make sure no mixing between receiving and sending messages happens, the service will always expect to receive a specific benthernet token.

```
When sending a request: "tcg?>"
When returning a response: "tcg!>"
```

### General commands

Some commands like logging in and getting a list of all active players are the same for all players. These functions are called general commands.

The general commands are constructed as listed below. Also listed below is a list of all the possible general_command token's ( this is the same list when a user uses  the "tcg?>help>" general command)

```
-benternet token       
-general_command token 
-( extra token's )
->

list  of all general command's:
    - login>(username)>
    - signup>(username)>
    - help>
```

### user specific commands

Most of the commands however are user specific. For example if a user want's to use mana, the service must know which user's mana to use.

Listed below is the way a user can construct messages that the service will use to manage the players mana_count, cards, tools, lifepoints, ... also listed below are all the commands users can use.

```
- benternet token
- user_name token
- action token       
- ( extra token's )
- >

list of all user specific command's:
    -(username)>help>
    -(username)>get mana count>
    -(username)>get max mana>
    -(username)>gather mana>

```

If an invalid user is used, for example the user is called "quinten" but tries using "quinte" the service will return a general command telling the "wrong user name" that they used an invalid username. 

!!! All player's that aren't loged in will also receive this message. !!!

## Examples

### Login and signup

```
tcg?>signup>quinten>
    tcg!>quinten>succesfull signup>
tcg?>quinten>get mana count>
    tcg!>quinten>not yet logged in, use "login>" first>
tcg?>login>quinte>
    tcg!>login>not a valid username>
tcg?>login>quinten>
    tcg!>quinten>succesfull login>
...
```

