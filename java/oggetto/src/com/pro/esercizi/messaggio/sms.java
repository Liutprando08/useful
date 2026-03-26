package com.pro.esercizi.messaggio;

import com.pro.esercizi.interfaces.archiviabile;

public class sms extends messaggio implements archiviabile {

  protected String numeroTelefono;

  @Override
  public void invia() {
    System.out.println("SMS inviato al numero " + numeroTelefono);

  }

  public sms(int id, String destinatario, String testo, String numeroTelefono) {
    super(id, destinatario, testo);
    this.numeroTelefono = numeroTelefono;
  }

  public String getNumeroTelefono() {
    return numeroTelefono;
  }

  public void setNumeroTelefono(String numeroTelefono) {
    this.numeroTelefono = numeroTelefono;
  }

  @Override
  public String formattaPerFile() {
    return "SMS|" + id + "|" + destinatario + "|" + numeroTelefono + "|" + testo;
  }

}
