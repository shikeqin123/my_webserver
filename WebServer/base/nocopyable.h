#pragma once

//不可复制的基类
class nocopyable{
protected:
    nocopyable(){};
    ~nocopyable(){};
private:
    nocopyable(const nocopyable &);
    const nocopyable& operator =(const nocopyable &);
};