void compilemyclass(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }
  gSystem->CompileMacro("Vector3D.cxx",opt.Data());
  gSystem->CompileMacro("Atom.cxx",opt.Data());
  gSystem->CompileMacro("Network.cxx",opt.Data());
  gSystem->CompileMacro("Chain.cxx",opt.Data());
  gSystem->CompileMacro("MyRandom.cxx",opt.Data());
  gSystem->CompileMacro("Folder.cxx",opt.Data());
  gSystem->CompileMacro("Fold_protein.C",opt.Data());
}