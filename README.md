# tcg service

## Trading Card Game

a game that is based around collecting and fighting other cards

players will be able to collect mana, this mana can be used to get cards and play them. some cards will make the player receive more mana or upgrade the amount of mana they can have at maximum. this while they can be attacked by other players if they wish to engage in battle.

## message construction

received messages will automatically be tokenized, therefore the message will be constructed as one of the two following ways:

    -benternet token    -benternet token
    -user_name token    -general_command token
    -action token       -extra token's
    -extra token's      ...
    ...

however, the service will cut or add the "benternet token" accordingly

if writing a client, it is expected to follow the same principals of constructing messages

## cards

there are three main type of cards, tools, spells and monsters. tools can be used to upgrade certain values like maximum mana or the amount of monsters someone can have in play, while spells are instant effects that can do various things. then there are monsters wich need mana to be summoned and can be used for defence or attack.

## life points

players need to be carefull though, as they can be attacked by other players at random. if a player loses all of there life points, they will lose some of the cards they collected, wich can lead to defestating chains if they lose the card witch upgraded the amount of mana they got every action.

