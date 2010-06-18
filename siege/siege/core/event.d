/**
    \brief Event handling
    \todo Each of "held down" events has a "down" argument - it seems to be rather reduntant...
*/
module siege.core.event;

private
{
    import siege.core.core;
    import siege.core.entity;

    import siege.util.linkedlist;

    import siege.physics.pbody;
}

/**
    \brief Holds the "core" events - that is, events related with the most basic game control
*/
interface CoreEventClient
{
    /// \brief Called when \ref siege.core.core.Game.run "game.run" is called
    void evStart();
    /// \brief Called when \ref siege.core.core.Game.run "game.run" exits, usually via \ref siege.core.core.Game.exit "game.exit"
    void evExit();

    /* @{ */
    /// \brief Second event to be called in each loop
    void evTick();
    /// \brief First event to be called in each loop
    void evTickBegin();
    /// \brief Third event to be called in each loop
    void evTickEnd();
    /// \brief Fourth event to be called in each loop - drawing should be done in here
    void evDraw();
    /* @} */
}

/**
    \brief Holds module-related events - these are called on \ref siege.core.core.Game.init "game.init" and \ref siege.core.core.Game.deinit "game.deinit"
*/
interface ModuleEventClient
{
    /// \brief Called on \ref siege.core.core.Game.init "game.init"
    void evInit();
    /// \brief Called on \ref siege.core.core.Game.deinit "game.deinit"
    void evDeinit();
}

/**
    \brief Holds window-related events - closing, opening and resizing the window
    \todo Window focus events
*/
interface WindowEventClient
{
    /* @{ */
    /// \brief Window has been opened
    void evWindowOpen();
    /// \brief Window has been closed
    void evWindowClose();
    /* @} */

    /**
        \brief Window has been resized
        \param width New window width
        \param height New window height
    */
    void evWindowResize(uint width, uint height);
}

/**
    \brief Holds mouse-related events - mouse buttons, motion, wheel
    \todo Local mouse events (events when the mouse is over an item)
    \todo What do with do with the mouse wheel? It seems that most APIs detect the wheel as buttons, should we do the same?
*/
interface MouseEventClient
{
    /* @{ */
    /**
        \brief A mouse button is being held down
        \param button The button ID
        \param down The button state
    */
    void evMouseButton(uint button);
    /**
        \brief A mouse button has just been pressed
        \param button The button ID
    */
    void evMouseButtonPress(uint button);
    /**
        \brief A mouse button has just been released
        \param button The button ID
    */
    void evMouseButtonRelease(uint button);
    /* @} */

    /* @{ */
    /**
        \brief Left mouse button is being held down
        \param down The button state
    */
    void evMouseButtonLeft(bool down);
    /// \brief Left mouse button has just been pressed
    void evMouseButtonLeftPress();
    /// \brief Left mouse button has just been released
    void evMouseButtonLeftRelease();
    /* @} */

    /* @{ */
    /**
        \brief Middle mouse button is being held down
        \param down The button state
    */
    void evMouseButtonMiddle(bool down);
    /// \brief Middle mouse button has just been pressed
    void evMouseButtonMiddlePress();
    /// \brief Middle mouse button has just been released
    void evMouseButtonMiddleRelease();
    /* @} */

    /* @{ */
    /**
        \brief Right mouse button is being held down
        \param down The button state
    */
    void evMouseButtonRight(bool down);
    /// \brief Right mouse button has just been pressed
    void evMouseButtonRightPress();
    /// \brief Right mouse button has just been released
    void evMouseButtonRightRelease();
    /* @} */

    /**
        \brief The mouse has been moved
        \param x New x mouse position in window coordinates
        \param y New y mouse position in window coordinates
    */
    void evMouseMove(int x, int y);
    /**
        \brief The mouse wheel has been moved
        \param wheel New wheel position
    */
    void evMouseWheel(int wheel);
}

/**
    \brief Holds keyboard-related events
*/
interface KeyboardEventClient
{
    /* @{ */
    /**
        \brief A key is being held down
        \param key The key ID
        \param down The key state
    */
    void evKeyboardKey(uint key);
    /**
        \brief A key has just been pressed
        \param key The key ID
    */
    void evKeyboardKeyPress(uint key);
    /**
        \brief A key has just been released
        \param key The key ID
    */
    void evKeyboardKeyRelease(uint key);
    /**
        \brief A keypress has just been repeated
        \param key The key ID
        \todo Make this work!
    */
    void evKeyboardKeyRepeat(uint key);
    /* @} */

    /* @{ */
    /**
        \brief A key representing a character has just been pressed
        \param chr The character (UTF-32)
    */
    void evKeyboardCharPress(dchar chr);
    /**
        \brief A keypress representing a character has just been repeated
        \param chr The character (UTF-32)
    */
    void evKeyboardCharRepeat(dchar chr);
    /* @} */
}

/**
    \brief Holds joystick-related events
*/
interface JoystickEventClient
{
    /* @{ */
    /**
        \brief A joystick button is being held down
        \param joy The joystick ID
        \param button The button ID
        \param down The button state
    */
    void evJoystickButton(uint joy, uint button);
    /**
        \brief A joystick button has just been pressed
        \param joy The joystick ID
        \param button The button ID
    */
    void evJoystickButtonPress(uint joy, uint button);
    /**
        \brief A joystick button has just been released
        \param joy The joystick ID
        \param button The button ID
    */
    void evJoystickButtonRelease(uint joy, uint button);
    /* @} */

    /**
        \brief The joystick axis have moved
        \param joy The joystick ID
        \param pos The axis positions
    */
    void evJoystickMove(uint joy, float[] pos);
}

/**
    \brief Holds network-related events
    \todo to be done
*/
interface NetworkEventClient
{
}

/**
    \brief Holds physics and collision-related events
*/
interface PhysicsEventClient
{
    /* @{ */
    /**
        \brief Two objects are in collision with one another
        \param other The other object
        \param point Collision point info
    */
    void evCollision(Entity other, CollisionPoint point);
    /**
        \brief Collision event for the first of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionOne(Entity other, CollisionPoint point);
    /**
        \brief Collision event for the second of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionTwo(Entity other, CollisionPoint point);
    /* @} */

    /* @{ */
    /**
        \brief Two objects have just come in collision with one another
        \param other The other object
        \param point Collision point info
    */
    void evCollisionBegin(Entity other, CollisionPoint point);
    /**
        \brief Collision start event for the first of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionOneBegin(Entity other, CollisionPoint point);
    /**
        \brief Collision start event for the second of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionTwoBegin(Entity other, CollisionPoint point);
    /* @} */

    /* @{ */
    /**
        \brief Two objects are no longer in collision with one another
        \param other The other object
        \param point Collision point info
    */
    void evCollisionEnd(Entity other, CollisionPoint point);
    /**
        \brief Collision end event for the first of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionOneEnd(Entity other, CollisionPoint point);
    /**
        \brief Collision end event for the second of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionTwoEnd(Entity other, CollisionPoint point);
    /* @} */

    /* @{ */
    /**
        \brief The result of the collision has been computed
        \param other The other object
        \param point Collision result info
    */
    void evCollisionResult(Entity other, CollisionResult result);
    /**
        \brief Collision result event for the first of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionOneResult(Entity other, CollisionResult result);
    /**
        \brief Collision result event for the second of two objects
        \param other The other object
        \param point Collision point info
    */
    void evCollisionTwoResult(Entity other, CollisionResult result);
}

/**
    \brief Holds level-related events
    \todo to be done
*/
interface LevelEventClient
{
    /// \brief A level has just loaded and started
    void evLevelStart();
    /// \brief A level has just ended
    void evLevelEnd();
}

/**
    \brief Contains event stubs, so that the compiler does not report about missing methods
*/
abstract class EventClient: CoreEventClient, ModuleEventClient, WindowEventClient, MouseEventClient, KeyboardEventClient, JoystickEventClient, NetworkEventClient, PhysicsEventClient, LevelEventClient
{
    bool active;
    LinkedNode!(EventClient) *node;

    this()
    {
        active = true;
        node = clientList.append(this);
    }
    ~this()
    {
        clientList.remove(node);
    }

    /// \brief Activate the EventClient, thus making it accept events (default)
    void activate()
    {
        active = true;
    }
    /// \brief Deactivate the EventClient, making it no longer accept events
    void deactivate()
    {
        active = false;
    }

    void evStart() {}
    void evExit() {}
    void evTick() {}
    void evTickBegin() {}
    void evTickEnd() {}
    void evDraw() {}

    void evInit() {}
    void evDeinit() {}

    void evWindowOpen() {}
    void evWindowClose() {}
    void evWindowResize(uint width, uint height) {}

    void evMouseButton(uint button) {}
    void evMouseButtonPress(uint button) {}
    void evMouseButtonRelease(uint button) {}
    void evMouseButtonLeft(bool down) {}
    void evMouseButtonLeftPress() {}
    void evMouseButtonLeftRelease() {}
    void evMouseButtonMiddle(bool down) {}
    void evMouseButtonMiddlePress() {}
    void evMouseButtonMiddleRelease() {}
    void evMouseButtonRight(bool down) {}
    void evMouseButtonRightPress() {}
    void evMouseButtonRightRelease() {}
    void evMouseMove(int x, int y) {}
    void evMouseWheel(int wheel) {}

    void evKeyboardKey(uint key) {}
    void evKeyboardKeyPress(uint key) {}
    void evKeyboardKeyRelease(uint key) {}
    void evKeyboardKeyRepeat(uint key) {}
    void evKeyboardCharPress(dchar chr) {}
    void evKeyboardCharRepeat(dchar chr) {}

    void evJoystickButton(uint joy, uint button) {}
    void evJoystickButtonPress(uint joy, uint button) {}
    void evJoystickButtonRelease(uint joy, uint button) {}
    void evJoystickMove(uint joy, float[] pos) {}

    void evCollision(Entity other, CollisionPoint point) {}
    void evCollisionOne(Entity other, CollisionPoint point) {}
    void evCollisionTwo(Entity other, CollisionPoint point) {}

    void evCollisionBegin(Entity other, CollisionPoint point) {}
    void evCollisionOneBegin(Entity other, CollisionPoint point) {}
    void evCollisionTwoBegin(Entity other, CollisionPoint point) {}

    void evCollisionEnd(Entity other, CollisionPoint point) {}
    void evCollisionOneEnd(Entity other, CollisionPoint point) {}
    void evCollisionTwoEnd(Entity other, CollisionPoint point) {}

    void evCollisionResult(Entity other, CollisionResult result) {}
    void evCollisionOneResult(Entity other, CollisionResult result) {}
    void evCollisionTwoResult(Entity other, CollisionResult result) {}

    void evLevelStart() {}
    void evLevelEnd() {}
}
