local enemy = {}

--Set variables
local entityID
local speed
local moveDelta
local grounded
local collided
local yVeclocity
local maxVelocity
local minVelocity 
local gravity
local moveRight
local moveTimer
local timeToMove
local goingOverEdge

function enemy:start()
    --Called once
    entityID = self.ID
    grounded = false
    collided = false
    goingOverEdge = false
    speed = 50
    moveDelta = vector(0,0,0)
    yVeclocity = 0.0
    maxVelocity = 100.0
    minVelocity = -100.0
    gravity = 2.0
    moveRight = true
    moveTimer = 0.0
    timeToMove = 2.0
end

function enemy:update(delta)
    --Called once per frame
    moveTimer = moveTimer + delta

    if collided then 
        moveTimer = timeToMove + 1.0
    end

    if moveTimer > timeToMove then
        moveTimer = 0.0
        moveRight = not moveRight
    end


    local trans = scene.GetComponent(entityID, "Transform")
     local direction = vector(0,0,0)

    if moveRight then
        direction = vector(1,0,0)
    else
        direction = vector(-1,0,0)
    end
    moveDelta = direction * speed * delta
    trans.position = trans.position + moveDelta


    
    if not grounded then
        if yVeclocity > minVelocity then
            yVeclocity =  yVeclocity - gravity
        end
    end
    if grounded then
        yVeclocity = 0.0
    end
    trans.position.y = trans.position.y + yVeclocity * delta
    
    scene.SetComponent(entityID, "Transform", trans)
    scene.SetCamPos(trans.position + vector(0,30,150))
    grounded = false
    collided = false

    --Remove enteties who have fallen of the map
    if trans.position.y < -200.0 then
        scene.RemoveEntity(entityID)
    end
end

function enemy:onCollision(what)
    --called on collision
    --print("Collision "..what)
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = trans.position - moveDelta * 1.0
    scene.SetComponent(entityID, "Transform", trans)
    moveDelta = vector(0,0,0)
    collided = true
end

function enemy:rayIntersect()
    --print("Grounded!")
    grounded = true
end



function enemy:reset()
    print("RESET")
    local trans = scene.GetComponent(entityID, "Transform")
    trans.position = vector(10, 30, 0)
    scene.SetComponent(entityID, "Transform", trans)
end

return enemy