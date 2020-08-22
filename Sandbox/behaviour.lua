function onSpriteClicked(sprite, x, y)
	
	_MoveSprite(sprite, x+0.1, y+0.1)
	print("Sprite was clicked at x=" .. x .. ", y=" .. y)
end

function onCollision(player, other)
	destroyThing(other)
	print("Collision happened")
end