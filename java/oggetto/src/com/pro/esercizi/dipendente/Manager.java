public class Manager extends Dipendente {
    private double bonusAnnuo;

    public Manager(String nome, String cognome, double stipendioBaseMensile, double bonusAnnuo) {
        super(nome, cognome, stipendioBaseMensile);
        this.bonusAnnuo = bonusAnnuo;
    }

    public double getBonusAnnuo() {
        return bonusAnnuo;
    }

    @Override
    public double calcolaStipendioAnnuo() {
        return (getStipendioBaseMensile() * 12) + bonusAnnuo;
    }

    @Override
    public String toString() {
        return "Manager{" +
                "nome='" + getNome() + '\'' +
                ", cognome='" + getCognome() + '\'' +
                ", stipendioBaseMensile=" + getStipendioBaseMensile() +
                ", bonusAnnuo=" + bonusAnnuo +
                ", stipendioAnnuo=" + calcolaStipendioAnnuo() +
                '}';
    }
}
