/**
    \brief Holds the \ref Entity class
*/
module siege.core.entity;

private
{
    import siege.util.vector;
    import siege.graphics.sprite;
    import siege.graphics.draw;

    import siege.physics.pbody;
    import siege.physics.shape;
    import siege.physics.space;
}

/**
    \brief This is the class which should be used for all objects which directly interact in the game (player, enemies, powerups...)
    \note Should not be used directly, but is not declared as abstract for testing purposes
*/
class Entity: Body
{
    Sprite sprite;
    Sprite mask;
    Shape shape;

    /* @{ */
    this(Vector position, float angle, Sprite sprite, Sprite mask = null, bool usePhysics = true)
    {
        this.sprite = sprite;
        this.mask = mask;

        BodyData data;
        data.position = position;
        data.angle = angle;
        data.usePhysics = usePhysics;

        this(data);
    }

    this(BodyData data, Space space = null)
    {
        assert(sprite !is null); // needed for size

        if(mask is null)
            mask = sprite;

        super(data);

        if(data.usePhysics)
        {
            if(space is null)
                space = space.main;

            Vector size = Vector(sprite.size.x, sprite.size.y) / 2;

            shape = new PolyShape(this, Vector(0, 0), Vector(-size.x, size.y), -size, Vector(size.x, -size.y), size);
            space ~= this;
            space ~= shape;
        }
    }
    /* @} */

    /* @{ */
    void evDraw()
    {
        if(sprite !is null)
        {
            draw.color(1.0, 1.0, 1.0, 1.0);
            sprite.draw(position, Vector(1.0, 1.0), angle);
        }
    }
    /* @} */
}
