function setSetting(record) {
    return wdimgui.setSetting(
        {
            "textColor": [1.0, 1.0, 1.0],
            "buttonSetting": {
                "buttonColor": [0.5, 0.5, 0.5],
                "hoverButtonColor": [0.5, 0.0, 1.0],
                "clickButtonColor": [0.5, 1.0, 0.0]
            },
            "radioButtonSetting": {

                "radioButtonOuterColor": [0.3, 0.3, 0.3],
                "radioButtonInnerColor":



                    [0.15, 0.15, 0.15],


                "radioButtonOuterColorHover":
                    [0.33, 0.33, 0.33],


                "radioButtonInnerColorHover":

                    [0.15, 0.15, 0.15],

                "radioButtonCircleSegments":

                    9,

                "radioButtonInnerRadius": 0.6,

                "radioButtonOuterRadius":
                    1.0
            },


            "fontTexUvForWhite": [0.5, 0.5]
        }, record
    );
}