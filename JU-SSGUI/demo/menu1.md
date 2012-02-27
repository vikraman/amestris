 MENU: [103];

 "Main" [6]{
 	"File" [6] {
        	"About" [11];
        	"Change Password" [12];
        	"Plot" [4] {
                     "Health Linearity TEST" [4] {
                                           "Ftc controls" [1311];
                                           "Sitvc controls" [1312];
                                           "Fnc controls" [1313];
                                           "Rec controls" [1314];
                                                }
                     "Null Linearity TEST" [4]{
                                           "Ftc pressures" [1321];
                                           "Sitvc pressures" [1322];
                                           "Fnc pressures" [1323];
                                           "Rec pressures" [1324];
                                             }
                     "Auto Press" [14];
		     "stop Plot" [15];
			   }
		"Print" [12] {
		     "All Ist STAGE PRESSURES" [161];
		     "All PAYLOAD PRESSURE" [162];
                     "All BAT VOLTAGE" [163];
                     "All Ist STAGE FBS" [164];
                     "All IInd STAGE FBS" [165];
                     "All PAYLOAD FBK+BS" [166];
                     "All MIS/LIU VOLTAGES" [167];
                     "All LIU CURRENTS" [168];
                     "All SURU PARAM" [169];
                     "AUTO PRESS PARA" [170];
                     "AUTO LEVEL DATA" [171];
                     "All CONTROL NULLS" [172];
                             }
 		
		\SEPARATOR;
        	"Quit"  [17];
   		}

 	"Entry" [3] {
       		"Plot" [3]{
                     "MD Console" [211];
                     "VD Console" [212];
                     "TEST" [213];
                      }
                "Print" [22];
                "Display" [23];
                 } 
       "CONFIG" [10] {
      		"Sioport" [31];
      		"Pi port "[32];
      		"Po port" [33];
      		"MD COnsole Enable"[34];
      		"VD Console Enable" [35];
      		"PL Console Enable" [36];
      		"RS422 Loop Test" [37];
      		"MD Console Disable"[38];
      		"VD Console Disable" [39];
      		"Pl Console Disable"[310];
      		  }
       "CALC" [4] {
                "Hex to deci" [41];
                "Deci to Hex" [42];
                "Add Sub" [43];
                "Fun" [44];
                 }
       "ACTION" [6] {
                "Non real-time" [51];
                "Checkout" [7]{
                         "UTILITY" [7]{
                                 "OBC" [6]{
                                    "Virtual Console" [52111];
                                    "Reset" [52112];
                                    "Load" [52113];
                                    "Start Autolaunch" [52114];
                                    "Self Check" [52115];
                                    "Obc_Surv_Stop" [52116];
                                           }
                                 "LIU" [6] {
                                     "LIU Console" [52121];
                                     "Reset" [52122];
                                     "Load" [52123];
                                     "Invoke" [52124];
                                     "Self Check" [52125];
                                     "Liu_Surv_Stop" [52126];
                                           }       
                                 "LIU POWER" [3]{
                                     "POWER CMDS" [8]{
                                         "MIU_OBC_INT_SUPP" [3]{
						"ENABLE" [5213111];
						"DISABLE" [521312];
						"NULL" [521313];
								    }	
                                         "HPIGS_THEIR_SUP" [3]{
						"ENABLE" [5213121];
						"DISABLE" [5213122];
						"NULL" [5213123];
								   }	
                                         "CPIF1_SCP_1501_SUP" [3]{
						"ENABLE" [5213131];
						"DISABLE" [5213132];
						"NULL" [5213133];
								       }	
                                         "RES_HPIGS_HIG_SUP" [3]{
						"ENABLE" [5213141];
						"DISABLE" [5213142];
						"NULL" [5213143];
								     }	
                                         "CPIF3/RCS_SUP" [3]{
						"ENABLE" [5213151];
						"DISABLE" [5213152];
						"NULL" [5213153];
								 } 
                                         "TMI/TCI & II_SUP" [3]{
						"ENABLE" [5213161];
						"DISABLE" [5213162];
						"NULL" [5213163];
								   } 
                                         "TMII_TRANS_SUP" [3]{
						"ENABLE" [5213171];
						"DISABLE" [5213172];
						"NULL" [5213173];
								  }
                                         "TMII_TRANS_SUP" [3]{
						"ENABLE" [5214181];
						"DISABLE" [5214182];
						"NULL" [5214183];
								  }	  
                                                     } 
                                     "VALVE CMDS" [6]{
                                         "HPSV_CMD" [3]{
						"OPEN" [5213211];
						"CLOSE" [5213212];
						"null" [5213213];
                   				       }	
				         "LPSV_CMD" [3]{
						"OPEN" [5213221];
						"CLOSE" [5213222];
						"null" [5213223];
						       }	
                                         "OLSV_CMD" [3]{
						"OPEN" [5213231];
						"CLOSE" [5213232];
						"null" [5213233];
							}
                                         "SYSTEM_START_VALVE" [3]{
						"OPEN" [5213241];
						"CLOSE" [5213242];
						"null" [5213243];
								  }
                                         "BYPASS_VALVE" [3]{
						"OPEN" [5213251];
						"CLOSE" [5213252];
						"null" [5213253];
							   }
                                         "LATCH_VALVE" [3]{
						"OPEN" [5213261];
						"CLOSE" [5213262];
						"null" [5213263];
							   }	
                                                     }      
                                     "null" [52133];
                                                } 
                                 "LIU PYRO" [3]{
					 "PYRO NORMAL" [3]{
  						"AT_PYRO_V_SUP_" [3]{
							"ARM" [5214111];
							"SAFE" [5214112];
							"null" [5214113];
                                                                    } 
						"S1_IGN_SUP_N_" [3]{
							"ARM" [5214121];
							"SAFE" [5214122];
							"null" [5214123];
								  }	
						"FLT_PYRO_SUP_N" [3]{
							"ARM" [5214131];
							"SAFE" [5214132];
							"null" [5214133];
								    }	
							  }		
					 "PYRO REDUNDANT" [3]{
						"AT_PYRO_V_SUP_" [3]{
							"ARM" [5214211];
							"SAFE" [5214212];
							"null" [5214213];
								     } 	
						"S1_IGN_SUP_R" [3]{
							"ARM" [5214221];
							"SAFE" [5214222];
							"null" [5214223];
								   }	
						"FLT_PYRO_SUP_R" [3]{
							"ARM" [5214231];
							"SAFE" [5214232];
							"null" [5214233];
								    }	
						             }		
					 "PYRO TELMTY" [3]{
						"CDS ARM CMDS" [4]{
							"CDS_I_BAT_ARM" [5214311];
							"CDS_I_PYRO_ARM" [5214312];
							"CDS_II_BAT_ARM" [5214313];
							"CDS_II_PYRO_ARM" [5214314];
								 }

						"CDS SAFE CMDS" [3]{
							"CDS_I_BAT_PYRO_SAFE" [5214321];
							"CDS_II_BAT_PYRO_SAFE" [5214322];
							"null" [5214323];
									}
						"null" [521433];
						          }		
						}	
                                 "Liu Comp" [6]{
					"S1_IGNITION_CMDI_II_N" [52151];
					"S1_IGNITION_CMDI_II_R" [52152];
					"ALL LIU POWER" [52153];
					"ALL LIU PYRO" [52154];
					"ALL LIU CDS" [52155];
					"ALL LIU VALUES" [52156];
					       }	
                                 "Tg" [5216];
                                 "Check Null limits" [5217];
                                       }
                         "POWER" [7]{
				"Composite" [6]{
					"Pay Load Mtr" [3]{
						"ENABLE" [522111];
						"DISABLE" [522112];
						"NULL" [522113];
							  }
					"ALL EXT" [3]{
						"ON" [522121];
					        "OFF" [522122];
						"NULL" [522123];
						     } 
					"ALL INT" [3]{
						"ON" [522131];
					        "OFF" [522132];
						"NULL" [522133];
						     }
					"ALL T3 EXT" [3]{
						"ON" [522141];
					        "OFF" [522142];
						"NULL" [522143];
						        }	
					"ALL T3 INT" [3]{
						"ON" [522151];
					        "OFF" [522152];
						"NULL" [522153];
						        } 	
					"DC MOTORS" [3]{
						"ON" [522161];
					        "OFF" [522162];
						"NULL" [522163];
						       }	
						}
				"Mis Rel MIU2" [3]{
					"S2_TORQUE_MOTOR" [3]{
						"ON" [522211];
					        "OFF" [522212];
						"NULL" [522213];
							     }	
					"S1_PUMP_MOTOR_CMDR" [3]{
						"ON" [522221];
					        "OFF" [522222];
						"NULL" [522223];
								}
					"null" [52223];
						  }		
				"Mis Rel MIU3" [7]{
					"CPIF_4_EXT" [3]{
						"ON" [522311];
					        "OFF" [522312];
						"NULL" [522313];
							}
					"HPIGCS/SV/HIG/ACC_ET" [3]{
						"ON" [522321];
					        "OFF" [522322];
						"NULL" [522323];
								  }
					"CPIF_III_EXT" [3]{
						"ON" [522331];
					        "OFF" [522332];
						"NULL" [522333];
							  }	
					"PL_PUMP_MOTOR" [3]{	
						"ON" [522341];
					        "OFF" [522342];
						"NULL" [522343];
							   }
					"CPIF_II_EXT" [3]{
						"ON" [522351];
					        "OFF" [522352];
						"NULL" [522353];
							  }
					"CPIF_I_EXT" [3]{
						"ON" [522361];
					        "OFF" [522362];
						"NULL" [522363];
							}
					"PL_ELEC_INT" [3]{
						"ON" [522371];
					        "OFF" [522372];
						"NULL" [522373];
							 }
						  }
				"Tel Rel MIU2" [8]{
					"TMI_PCM_EXTERNAL" [3]{
						"ON" [522411];
					        "OFF" [522412];
						"NULL" [522413];
							      }	
					"TMI_TRX_EXTERNAL" [3]{
						"ON" [522421];
					        "OFF" [522422];
						"NULL" [522423];
							      }	
					"TCI_EXTERNAL" [3]{
						"ON" [522431];
					        "OFF" [522432];
						"NULL" [522433];
							  } 	
					"TCII_EXTERNAL" [3]{
						"ON" [522441];
					        "OFF" [522442];
						"NULL" [522443];
							   }	
					"TMI_PCM_INTERNAL" [3]{
						"ON" [522451];
					        "OFF" [522452];
						"NULL" [522453];
							      }	
					"TMI_TRX_INTERNAL" [3]{
						"ON" [522461];
					        "OFF" [522462];
						"NULL" [522463];
							      }	
					"TCI_INTERNAL" [3]{
						"ON" [522471];
					        "OFF" [522472];
						"NULL" [522473];
							  }
					"TCII_INTERNAL" [3]{
						"ON" [522481];
					        "OFF" [522482];
						"NULL" [522483];
							   }
						  }
				"Tel Rel MIU3" [8]{
					"TMII_SCP/PCM_EXT" [3]{
						"ON" [522511];
					        "OFF" [522512];
						"NULL" [522513];
                                                              }
					"TMII_T	RX_EXTERNAL" [3]{
						"ON" [522521];
					        "OFF" [522522];
						"NULL" [522523];
							        }	
					"TRANSP_CBAND_EXT" [3]{
						"ON" [522531];
					        "OFF" [522532];
						"NULL" [522533];
							      }	
					"TRANSP_SBAND_EXT" [3]{
						"ON" [522541];
					        "OFF" [522542];
						"NULL" [522543];
							      }
					"TMII_SCP/PCM_INT" [3]{
						"ON" [522551];
					        "OFF" [522552];
						"NULL" [522553];
							      }
					"TMII_TRX_INTERNAL" [3]{
						"ON" [522561];
					        "OFF" [522562];
						"NULL" [522563];
							       }	
					"TRANSP_CBAND_INT" [3]{
						"ON" [522571];
					        "OFF" [522572];
						"NULL" [522573];
							      }
                                        "TRANSP_SBAND_INT" [3]{  
						"ON" [522581];
					        "OFF" [522582];
						"NULL" [522583];
							      }
			                          } 
				"Int unit status" [5226];
				"Auto Rel Chk" [5]{
					"OBC" [52271];
					"MIU2" [52272];
					"MIU3" [52273];
					"INS" [52274];
					"LIU" [52275];
						     }	
				    }	
                         "PYRO" [10]{
				"Composite" [8]{
					"Gnd Ini Pyros(N)" [52311];
					"S1 Ing Pyros(N)" [52312];
					"S1 Ing Pyros(R)" [52313];
					"Flg Ini Pyros(N)" [52314];
					"Flg Ini Pyros(R)" [52315];
					"CDS PYROS" [52316];
					"Pyro Sequence(N) "[52317];
					"Pyro Sequence(R) "[52318];
						}	
				"System Battery" [3]{
					"ARM" [52321];
					"SAFE" [52322];
					"null" [52323];
						    }	
				"System Pyros" [3]{
					"ARM" [52331];
					"SAFE" [52332];
					"null" [52333];
						    }	
				"Mis Rel Miu2 Norm" [5]{
					"PL_AB_PV_COMMAND" [52341];	
					"S2_IGN_COMD" [52342];
					"S1_SEP_COMMAND" [52343];
					"RETRO MOTOR COMMAND" [52344];
					"S2_SEP_COMD" [52345];
						       }	

				"Mis Rel Miu2 Readn" [5]{
					"PL_AB_PV_COMMAND" [52351];	
					"S2_IGN_COMD" [52352];
					"S1_SEP_COMMAND" [52353];
					"RETRO MOTOR COMMAND" [52354];
					"S2_SEP_COMD" [52355];
					                   }
				"Mis Rel Miu3 Norm" [3]{
					"BAT_PV_PY_REL_ARM_N" [3]{
					   "ARM" [523611];
					   "SAFE" [523612];
					   "null" [523613];
							   }
	                
			                "S1_IGN_PY_REL_ARM_N" [3]{
					   "ARM" [523621];
					   "SAFE" [523622]; 
					   "null" [523623];
                                                           }
					"FLT_PY_REL_ARM_N" [3]{
					   "ARM" [523631];
					   "SAFE" [523632];
					   "null" [523633];
                                                         }  
			                              } 
				"Mis Rel Miu3 Readn" [3]{
			
					"BAT_PV_PY_REL_ARM_R" [3]{	
					   "ARM" [523711];
					   "SAFE" [523712];
					   "null" [523713];
                                                                 }
					"S1_IGN_PY_REL_ARM_R" [3]{
					   "ARM" [523721];
					   "SAFE" [523722]; 
					   "null" [523723];
								 }	
					"FLT_PY_REL_ARM_R" [3]{
					   "ARM" [523731];
					   "SAFE" [523732];
					   "null" [523733];
							      } 	
				                        }	


				"Mis Rel Miu3 Left" [5238];
				"Mis Rel Miu3 Right" [5239];
				"Auto Rel Check" [5240];
                                 }
                         "PRESSURE" [9]{
				"All Control Nulls" [5241];
				"All 1st Stage Pr" [5242];
				"All 2nd Stage pr" [5243];
				"All Payload Pr" [5244];
				"All Hydraulic Pr" [5245];
				"All MIS Voltages" [5246];
				"All TEL Voltages" [5247];
				"All Liu Voltages" [5248];
				"All Liu Currents" [5249];
					}
				
                         "CONTROL" [7]{
				"FTC" [5251];
				"SITVC" [5252];
				"FNC" [5253];
				"REC" [5254];
				"RCS" [5255];
				"PRINT" [5256];
				"ALL DAC ZERO" [5257];
				      }   	
                         "SENSOR" [5]{
				"SENSOR CHECK" [5261];
				"RGP CHECK" [5262];
				"START POS CHECK" [5263];
				"GET POS CHECK" [5264];
				"PRINT" [5265];
				        }	
                         "TELMET" [5]{
				"Invoke Term" [5271];
				"TEL ASSCENDING" [5272];
				"TEL DESENDING" [5273];
			        "TELL ALL 55" [5274];
				"TEL ALL FF" [5275];
				     } 	 	
                               }  
                "PreLaunch" [53];
                "AutoLaunch" [54];
                "Static mode" [55];
                "Post Lift Umb" [56];        
                 }
       "HELP" [1]{   
                "Help"[61];
                 }
        
           }
