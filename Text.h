#ifndef TEXT_H
#define TEXT_H


class Text {
public:
    Text();
    ~Text();

    char** getArray() const;
    void setArray(char** newArray);
    int getNrow() const;
    void setNrow(int newNrow);
    int getNCol() const;
    void setNCol(int newNcol);
    void initialize_array();
    void freeArray();
    void reallocate_rows();
    void newbuffer(size_t* buffersize);
    int getInitRows() const;

private:
    size_t buffersize;
    int initialrowcount;
    int nrow;
    int ncol;
    char** array;
};

#endif 
