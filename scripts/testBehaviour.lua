local test = {}

--Set variables
local moveRight
local entityID

function test:start()
    --Called once
    entityID = self.ID
    moveRight = true
   
    --print(entityID)
end

function test:update(delta)

    --Called once per frame
    local trans = scene.GetComponent(entityID, "Transform")

    if moveRight then
        trans.position.x = trans.position.x + 1 * delta
    else
        trans.position.x = trans.position.x - 1 * delta
    end

    if trans.position.x > 5 then
        print("Moving left!")
        moveRight = false
    elseif trans.position.x < -5 then
        print("Moving right!")
        moveRight = true
    end

    scene.SetComponent(entityID, "Transform", trans)

    scene.SetCamPos(trans.position + vector(0,0,-10))


end

return test