//
// Created by phoenixflower on 11/28/20.
//

#ifndef LANDESSDEVCORE_COUNTRIES_H
#define LANDESSDEVCORE_COUNTRIES_H
namespace LD
{
    enum CountryTag{Afghanistan,Albania,Algeria,Andorra,Angola,Anitgua,Barbuda,
            Argentina,Armenia,Australia,Austria,Azerbaijan,Bahamas,Bahrain,Bangladesh,
            Barbados,Belarus,Belgium,Belize,Benin,Bhutan,Bolivia,Bosnia,Herzegovina,
            Botswana,Brazil,Brunel,Bulgaria,BurkinaFaso,Burundi,Cotedlvoire,CaboVerde,
            Cambodia,Cameroon,Canada,CentralAfricanRepublic,Chad,Chile,China,Columbia,
            Comoros,Congo,CostaRica,Croatia,Cuba,Cyprus,CzechRepublic,DemocraticRepublicOfTheCongo,
            Denmark,Djibouti,Dominica,DominicanRepublic,Ecuador,Egypt,ElSalvador,
            EquatorialGuinea,Eritrea,Estonia,Eswatini,Ethiopa,Fiji,Finland,France,Gabon,
            Gambia,Georgia,Germany,Ghana,Greece,Grenada,Guatemala,Guinea,GuineaBissau,Guyana,Haiti,
            HolySee,Honduras,Hungary,Iceland,India,Indonesia,Iran,Iraq,Ireland,Isreal,Italy,
            Jamaica,Japan,Jordan,Kazakhastan,Kenya,Kiribati,Kuwait,Krygyzstan,Laos,Latvia,
            Lebanon,Lesotho,Liberia,Libya,Liechtenstein,Lithuania,Luxembourg,Madagascar,
            Malawl,Malaysia,Maidives,Mali,Malta,MarshallIslands,Mauritania,Mauritius,
            Mexico,Micronesia,Moldova,Monaco,Mongolia,Montenegro,Morocco,Mozambique,
            Myanmar,Namibia,Nauru,Nepal,Netherlands,NewZealand,Nicaragua,Niger,Nigeria,NorthKorea,
            NorthMacedonia,Norway,Oman,Pakistan,Palau,PalestineState,Panama,PapuaNewGuinea,Paraguay,
            Peru,Philippines,Poland,Portugal,Qatar,Romania,Russia,Rwanda,SaintKittsAndNevis,
            SaintLucia,SaintVincent,Grenadines,Samoa,SanMarino,SaoTome,Principe,SaudiArabia,Senegal,Serbia,
            Seychelles,SierraLeone,Singapore,Slovakia,Slovenia,SolomonIslands,Somalia,SouthAfrica,
            SouthKorea,SouthSudan,Spain,SriLanka,Sudan,Suriname,Swiden,Switzerland,Syria,Tajikistan,
            Tanzania,Thailand,TimorLeste,Togo,Tonga,Trinidad,Tobago,Tunisia,Turkey,Turkmenistan,
            Tuvalu,Uganda,Ukraine,UnitedArabEmirates,UnitedKingdom,UnitedStatesOfAmerica,
            Uruguay,Uzbekistan,Vanuatu,Venezuela,Vietnam,Yemen,Zambia,Zinbabwe};

    class Country
    {
    private:
        LD::CountryTag mCountry;
    public:
        Country(LD::CountryTag country) noexcept:mCountry{country}{}
    };
}
#endif //LANDESSDEVCORE_COUNTRIES_H
