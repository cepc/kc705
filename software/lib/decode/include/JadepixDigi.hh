#ifndef JADEPIXDIGI_H
#define JADEPIXDIGI_H


class JadepixDigi{
  public:
    JadepixDigi(){};
    JadepixDigi(int rowId, int colId, double adc):m_rowId(rowId),m_colId(colId), m_adc(adc){};
    ~JadepixDigi(){};

    void SetADC(double adc){m_adc = adc;};
    void SetRowId(int rowId){m_rowId = rowId;};
    void SetColId(int colId){m_colId = colId;};

    double GetADC() const {return m_adc;};
    int GetRowId() const {return m_rowId;};
    int GetColId() const {return m_colId;};

  private:
    int m_rowId;
    int m_colId;
    double m_adc;

};

#endif //JADEPIXDIGI_H