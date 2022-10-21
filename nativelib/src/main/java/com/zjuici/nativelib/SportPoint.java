package com.zjuici.nativelib;

public class SportPoint {
    public float x;
    public float y;
    public float pointScore;
    public int count;

    public SportPoint(float x, float y, float pointScore, int count) {
        this.x = x;
        this.y = y;
        this.pointScore = pointScore;
        this.count =count;
    }

    public float getX() {
        return x;
    }

    public void setX(float x) {
        this.x = x;
    }

    public float getY() {
        return y;
    }

    public void setY(float y) {
        this.y = y;
    }

    public float getPointScore() {
        return pointScore;
    }

    public void setPointScore(float pointScore) {
        this.pointScore = pointScore;
    }

    public int getCount(){
        return count;
    }

    public void setCount(int count){
        this.count =count;
    }
}
