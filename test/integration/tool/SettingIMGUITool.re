let setSetting =
    (
      ~record,
      ~textColor=[|1., 1., 1.|],
      ~buttonColor=[|1., 0.5, 0.3|],
      ~hoverButtonColor=[|1., 0.5, 0.3|],
      ~clickButtonColor=[|1., 0.5, 0.3|],
      ~fontTexUvForWhite=[|0.5, 0.|],
      (),
    ) =>
  ManageIMGUIAPI.setSetting(
    {
      "textColor": textColor,
      "buttonColor": buttonColor,
      "hoverButtonColor": hoverButtonColor,
      "clickButtonColor": clickButtonColor,
      "fontTexUvForWhite": fontTexUvForWhite,
    },
    record,
  );