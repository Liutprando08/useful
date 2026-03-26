package com.pro.esercizi.messaggio;

public abstract class messaggio {
  protected int id;
  protected String destinatario;
  protected String testo;

  public messaggio(int id, String destinatario, String testo) {
    this.id = id;
    this.destinatario = destinatario;
    this.testo = testo;
  }

  public abstract void invia();

  @Override
  public String toString() {
    return "messaggio [id=" + id + ", destinatario=" + destinatario + ", testo=" + testo + "]";
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getDestinatario() {
    return destinatario;
  }

  public void setDestinatario(String destinatario) {
    this.destinatario = destinatario;
  }

  public String getTesto() {
    return testo;
  }

  public void setTesto(String testo) {
    this.testo = testo;
  }

}
