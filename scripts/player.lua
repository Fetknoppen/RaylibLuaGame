local player = {}

--Set variables
local jump
local entityID
local speed
local moveDelta
local grounded
local collided
local yVeclocity
local jumpVelocity
local maxVelocity
local minVelocity 
local jumpForce
local gravity
local health

function player:start()
    --Called once
    entityID = self.ID
    health = 100
    jump = false;
    grounded = false
    collided = false
    speed = 100
    moveDelta = vector(0,0,0)
    yVeclocity = 0.0
    jumpVelocity = 0.0
    maxVelocity = 100.0
    minVelocity = -100.0
    jumpForce = 210.0
    gravity = 2.0
end

function player:update(delta)

    --Called once per frame
    local trans = scene.GetComponent(entityID, "Transform")
    local direction = vector(0,0,0)
    if (scene.IsKeyPressed("SPACE") and  grounded)then
        print("JUMP")
        jumpVelocity = jumpForce
    end
    if scene.IsKeyDown("A") then
        direction = direction + vector(-1,0,0)
        trans.rotation = vector(0,90,0)
    end
    if scene.IsKeyDown("D") then
        direction = direction + vector(1,0,0)
        trans.rotation = vector(0,-90,0)
    end

    if (not scene.IsKeyDown("D") and not scene.IsKeyDown("A")) then
        trans.rotation = vector(0,0,0)
    end


    moveDelta = direction * speed * delta
    trans.position = trans.position + moveDelta


    
    if not grounded then
        if yVeclocity > minVelocity then
            yVeclocity =  yVeclocity - gravity
        end
        if jumpVelocity > 0.0 then
            jumpVelocity = jumpVelocity - gravity
        end
    end
    if grounded then
        yVeclocity = 0.0
    end
    if collided then
        jumpVelocity = 0.0
    end
    --moveDelta = moveDelta + vector(0, yVeclocity*delta, 0)
    trans.position.y = trans.position.y + yVeclocity * delta
    trans.position.y = trans.position.y + jumpVelocity * delta
    
    scene.SetComponent(entityID, "Transform", trans)
    scene.SetCamPos(trans.position + vector(0,30,150))
    grounded = false
    collided = false

    if trans.position.x > scene.GetScreenWidth() then
        print("WIN!")
        print(scene.GetScreenWidth())
        scene.winGame()
    end
    if health <= 0 then
        print("LOOSE!")
        scene.winGame()
    end
end

function player:onCollision(who, what)
    --called on collision
    print("Collision "..what)
    if what == "player" then
        print("Player: collision with enemy")
        scene.RemoveEntity(who)
        health = health - 50
    end
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = trans.position - moveDelta * 1.0
    scene.SetComponent(entityID, "Transform", trans)
    moveDelta = vector(0,0,0)
    collided = true
end

function player:rayIntersect()
    --Calles when all ground rays are collidong with something
    grounded = true
end

function player:reset()
    print("RESET")
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = vector(10, 30, 0)
    scene.SetComponent(entityID, "Transform", trans)
end

return player