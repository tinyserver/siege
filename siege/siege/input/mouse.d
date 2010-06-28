module siege.input.mouse;

private
{
    import siege.core.core;
    import siege.core.event;
    import siege.core.window;

    import siege.util.misc;
    import siege.util.vector;
    import siege.util.linkedlist;

    import siege.modules.input;

    extern(C)
    {
        void cbMouseButton(void* cmouse, uint button, bool down)
        {
            mouse.button.update(button, down);

            bool pressed = siege.input.mouse.mouse.button[button].pressed;

            foreach(client; clientList)
                if((client !is null) && client.active)
                {
                    client.evMouseButton(button);
                    if(pressed)
                        client.evMouseButtonPress(button);
                    else
                        client.evMouseButtonRelease(button);

                    switch(button)
                    {
                        case 1:
                            client.evMouseButtonLeft();
                            if(pressed)
                                client.evMouseButtonLeftPress();
                            else
                                client.evMouseButtonLeftRelease();
                            break;
                        case 2:
                            client.evMouseButtonRight();
                            if(pressed)
                                client.evMouseButtonRightPress();
                            else
                                client.evMouseButtonRightRelease();
                            break;
                        case 3:
                            client.evMouseButtonMiddle();
                            if(pressed)
                                client.evMouseButtonMiddlePress();
                            else
                                client.evMouseButtonMiddleRelease();
                            break;
                        default:
                            break;
                    }
                }
        }
        void cbMouseMove(void* cmouse, int x, int y)
        {
            mouse.position.update(x, y);

            foreach(client; clientList)
                if((client !is null) && client.active)
                    client.evMouseMove(x, y);
        }
        void cbMouseWheel(void* cmouse, int w)
        {
            mouse.wheel.update(w);

            foreach(client; clientList)
                if((client !is null) && client.active)
                    client.evMouseWheel(w);
        }
    }
}

class MouseButton
{
    protected
    {
        bool prev;
        bool curr;

        void update(bool state)
        {
            prev = curr;
            curr = state;
        }
    }

    void opAssign(bool down)
    {
        assert(0, "UNIMPLEMENTED"); //TODO
    }

    bool previous()
    {
        return prev;
    }
    bool down()
    {
        return curr;
    }
    bool up()
    {
        return !curr;
    }
    bool pressed()
    {
        return !prev && curr;
    }
    bool released()
    {
        return prev && !curr;
    }
}

class MouseButtonHandle
{
    protected
    {
        MouseButton[] buttons;

        void update(ubyte button, bool state)
        {
            buttons[button].update(state);
        }
    }

    MouseButton left, middle, right;

    this(ubyte num)
    {
        num = max(num, 3U);

        buttons.length = num;
        foreach(inout button; buttons)
        {
            button = new MouseButton;
        }

        left = buttons[0];
        right = buttons[1];
        middle = buttons[2];
    }

    bool[] previous()
    {
       bool[] but;
       foreach(i, button; buttons)
       {
           but[i] = button.prev;
       }
       return but;
    }
    bool[] down()
    {
       bool[] but;
       foreach(i, button; buttons)
       {
           but[i] = button.down;
       }
       return but;
    }
    bool[] up()
    {
       bool[] but;
       foreach(i, button; buttons)
       {
           but[i] = button.up;
       }
       return but;
    }
    bool[] pressed()
    {
       bool[] but;
       foreach(i, button; buttons)
       {
           but[i] = button.pressed;
       }
       return but;
    }
    bool[] released()
    {
       bool[] but;
       foreach(i, button; buttons)
       {
           but[i] = button.released;
       }
       return but;
    }

    MouseButton opIndex(ubyte index)
    {
        return buttons[index];
    }
    MouseButton[] opSlice(ubyte index1, ubyte index2)
    {
        return buttons[index1..index2];
    }
    MouseButton[] opSlice()
    {
        return buttons[];
    }
}

class MousePosition
{
    protected
    {
        int[] prev;
        int[] curr;

        void update(int x, int y)
        {
            prev = curr.dup;
            curr = [x, y];
        }
    }

    this()
    {
        prev.length = 2;
        curr.length = 2;
    }

    /*void opAssign(Vector pos)
    {
        mouse.mouse.setPosition(cast(int)pos.x, cast(int)pos.y);
    }
    void opAssign(iVector pos)
    {
        mouse.mouse.setPosition(pos.x, pos.y);
    }*/

    Vector previous()
    {
        return Vector(prev[0], prev[1]);
    }
    Vector current()
    {
        return Vector(curr[0], curr[1]);
    }
    Vector delta()
    {
        return current() - previous();
    }
}

class MouseWheel
{
    protected
    {
        int curr;
        int prev;

        void update(int w)
        {
            prev = curr;
            curr = w;
        }
    }

    /*void opAssign(int pos)
    {
        mouse.mouse.setWheel(pos);
    }
    void opAssign(float pos)
    {
        mouse.mouse.setWheel(cast(int)pos);
    }*/

    int previous()
    {
        return prev;
    }
    int current()
    {
        return curr;
    }
    int delta()
    {
        return curr - prev;
    }
}

class Mouse
{
    //protected
    //{
        SGCoreMouseCallbacks callbacks;
        void* mhandle;
    //}

    MouseButtonHandle button;
    MousePosition position;
    MouseWheel wheel;

    this()
    {
        callbacks.button = &cbMouseButton;
        callbacks.move = &cbMouseMove;
        callbacks.wheel = &cbMouseWheel;

        if(sgmCoreMouseCreate !is null)
            sgmCoreMouseCreate(&mhandle, window.handle());

        if(sgmCoreMouseSetCallbacks !is null)
            sgmCoreMouseSetCallbacks(mhandle, &callbacks);

        uint buttons;
        if(sgmCoreMouseGetNumButtons !is null)
            sgmCoreMouseGetNumButtons(mhandle, &buttons);
        button = new MouseButtonHandle(buttons);
        position = new MousePosition;
        wheel = new MouseWheel;
    }
    ~this()
    {
        if(sgmCoreMouseDestroy !is null)
            sgmCoreMouseDestroy(mhandle);
    }

    void* handle()
    {
        return mhandle;
    }
}
Mouse mouse;
