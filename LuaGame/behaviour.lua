function onSpriteClicked(sprite, x, y)
	
	_MoveSprite(sprite, x+0.1, y+0.1)
	print("Sprite was clicked at x=" .. x .. ", y=" .. y)
end

function onCollision(player, other)
	destroyThing(other)
	
	y = math.random(-5,5) 
	x = math.random(-5,5) 
	createThing(game, "zerothree.png", x, y) 
	
	x = math.random(-5,5) 
	y = math.random(-5,5) 
	createThing(game, "zerothree.png", x, y) 

	print("Collision happened")
end