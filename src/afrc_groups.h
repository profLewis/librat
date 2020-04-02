#ifndef _AFRC_GROUPS_H
#define _AFRC_GROUPS_H

typedef struct {
	int   *jday_Ptr;
	float   *tmax_Ptr;
	float   *tmin_Ptr;
	float   *tdry_Ptr;
	float   *twet_Ptr;
	float   *rain_Ptr;
	float   *windr_Ptr;
	float   *sunhrs_Ptr;
	float   *rad_Ptr;
	float   *tt0_Ptr;
	float   *tt1_Ptr;
	float   *tt9_Ptr;
	float   *tmfac_Ptr;
	float   *tttwk_Ptr;
	float   *ttwk_Ptr;
	int   *ieday_Ptr;
	Data_type	data_type;
}Wether;
typedef struct {
	int   *iphase_Ptr;
	float   *photop_Ptr;
	float   *rfp_Ptr;
	float   *att_Ptr;
	float   *aptt_Ptr;
	float   *apvtt_Ptr;
	float   *rfv_Ptr;
	float   *daylen_Ptr;
	float   *pb_Ptr;
	float   *psat_Ptr;
	Data_type	data_type;
}Dev1; 
typedef struct {
	float   *tdse_Ptr;
	float   *tpvefi_Ptr;
	float   *tpfidr_Ptr;
	float   *tpdrts_Ptr;
	float   *tptsbe_Ptr;
	float   *tpbea_Ptr;
	float   *tdaas_Ptr;
	float   *tdgf_Ptr;
	float   *tdr_Ptr;
	Data_type	data_type;
}Dev2; 
typedef struct {
	float   *vb_Ptr;
	float   *vsat_Ptr;
	float   *topt_Ptr;
	float   *tb0_Ptr;
	float   *tb1_Ptr;
	float   *tb9_Ptr;
	float   *tdmax_Ptr;
	float   *tvmin_Ptr;
	float   *tvmax_Ptr;
	float   *tv1_Ptr;
	float   *tv2_Ptr;
	float   *tdv_Ptr;
	Data_type	data_type;
}Dev3;
typedef struct {
	int   *ipflag_Ptr;
	int   *iprint_Ptr;
	int   *iyflag_Ptr;
	int   *itoday_Ptr;
	int   *iystdy_Ptr;
	int   *iwkc_Ptr;
	int   *iwk1_Ptr;
	int   *itflag_Ptr;
	int   *icflag_Ptr;
	int   *iocflg_Ptr;
	int   *jslflg_Ptr;
	Data_type	data_type;
}Contrl;
typedef struct {
	int   *isowy_Ptr;
	int   *isowd_Ptr;
	int   *iharvd_Ptr;
	float   *zlat_Ptr;
	Data_type	data_type;
}Crop;
typedef struct {
	float   *pbdr_Ptr;
	float   *tba_Ptr;
	Data_type	data_type;
}Dev4;
typedef struct {
	float   *assnet_Ptr;
	float   *wtotal_Ptr;
	float   *qpabst_Ptr;
	float   *alai_Ptr;
	float   *rnasw_Ptr;
	float   *parw_Ptr;
	float   *co2net_Ptr;
	float   *cumpar_Ptr;
	float   *growte_Ptr;
	Data_type	data_type;
}Grow1;
typedef struct {
	float   *facr_Ptr;
	float   *face_Ptr;
	float   *facs_Ptr;
	float   *facl_Ptr;
	float   *facsp_Ptr;
	float   *totaln_Ptr;
	float   *totalg_Ptr;
	Data_type	data_type;
}Grow2;
typedef struct {
	float   *totr_Ptr;
	float   *tote_Ptr;
	float   *tots_Ptr;
	float   *totl_Ptr;
	float   *totg_Ptr;
	float   *spool_Ptr;
	float   *cgrsa_Ptr;
	float   *gnum_Ptr;
	Data_type	data_type;
}Grow3;
typedef struct {
	float   *alpha_Ptr;
	float   *gm_Ptr;
	float   *a_Ptr;
	float   *rt_Ptr;
	float   *ca_Ptr;
	float   *ra_Ptr;
	float   *theta_Ptr;
	float   *respf_Ptr;
	Data_type	data_type;
}Photo1;
typedef struct {
	float   *extk_Ptr;
	float   *delh_Ptr;
	float   *delhd_Ptr;
	float   *gscnst_Ptr;
	float   *deltas_Ptr;
	Data_type	data_type;
}Photo2;
typedef struct {
	float   *pi_Ptr;
	Data_type	data_type;
}Const;
typedef struct {
	float   *rldim_Ptr;
	float   *dtmax_Ptr;
	float   *eint_Ptr;
	float   *rmxtim_Ptr;
	float   *slmax_Ptr;
	float   *swidth_Ptr;
	float   *prmaxs_Ptr;
	float   *rln0_Ptr;
	float   *conslw_Ptr;
	float   *fn_Ptr;
	float   *elms_Ptr;
	float   *elt_Ptr;
	Data_type	data_type;
}Rlvsi;
typedef struct {
	float   *alaims_Ptr;
	float   *rlaims_Ptr;
	float   *rmslai_Ptr;
	float   *talams_Ptr;
	float   *tdalms_Ptr;
	float   *tdwlms_Ptr;
	float   *tlamss_Ptr;
	float   *rlvsm_Ptr;
	float   *rnlms_Ptr;
	int   *nlmcbn_Ptr;
	Data_type	data_type;
}Rlvsmo;
typedef struct {
	float   *alaitl_Ptr;
	float   *rlaitl_Ptr;
	float   *talatl_Ptr;
	float   *tdaltl_Ptr;
	float   *tdwltl_Ptr;
	float   *tillai_Ptr;
	float   *tlatil_Ptr;
	int   *nlvst_Ptr;
	float   *rlvst_Ptr;
	float   *rnltil_Ptr;
	Data_type	data_type;
}Rlvsto;
typedef struct {
	float   *asslr_Ptr;
	float   *parlra_Ptr;
	float   *rootll_Ptr;
	float   *salrl_Ptr;
	float   *aaroot_Ptr;
	float   *const1_Ptr;
	int   *inlayr_Ptr;
	int   *kount_Ptr;
	int   *nlayer_Ptr;
	float   *plrass_Ptr;
	float   *srlmax_Ptr;
	float   *srootd_Ptr;
	float   *srootl_Ptr;
	float   *srootn_Ptr;
	float   *wsroot_Ptr;
	float   *wlroot_Ptr;
	float   *seedd_Ptr;
	Data_type	data_type;
}Roots;
typedef struct {
	float   *rinpop_Ptr;
	float   *tstop_Ptr;
	float   *rmxtpr_Ptr;
	float   *rnseds_Ptr;
	float   *eprop_Ptr;
	float   *atill_Ptr;
	float   *alphtl_Ptr;
	float   *betatl_Ptr;
	float   *f_Ptr;
	float   *cohrt_Ptr;
	float   *rn_Ptr;
	float   *atttb1_Ptr;
	Data_type	data_type;
}Shooti;
typedef struct {
	int   *ncht_Ptr;
	float   *surpr_Ptr;
	int   *nlchts_Ptr;
	float   *rn3_Ptr;
	float   *totpop_Ptr;
	float   *surms_Ptr;
	Data_type	data_type;
}Shooto;
typedef struct {
	int   *iyear_Ptr;
	int  *maxjd_Ptr;
	int   *iwstrt_Ptr;
	Data_type	data_type;
}Leap;
typedef struct {
	float   *obslai_Ptr;
	int   *ideknt_Ptr;
	Data_type	data_type;
}Obs;
typedef struct {
	float   *srotla_Ptr;
	float   *rotlla_Ptr;
	Data_type	data_type;
}Roots2;
typedef struct {
	float   *t1lai_Ptr;
	float   *t2lai_Ptr;
	float   *rtlai_Ptr;
	float   *rnt1_Ptr;
	float   *rnt2_Ptr;
	float   *rnrsht_Ptr;
	Data_type	data_type;
}Tilout;
typedef struct {
	char	*site_Ptr;
	char	*varty_Ptr;
	Data_type	data_type;
}Sitvar;
typedef struct {
	float   *cmresp_Ptr;
	float   *cgresp_Ptr;
	Data_type	data_type;
}Resp;
typedef struct {
	int   *nlmsdr_Ptr;
	int   *nltldr_Ptr;
	int   *jldiff_Ptr;
	Data_type	data_type;
}Mslfn;
typedef struct {
	int   *mxnsht_Ptr;
	int   *inlvs_Ptr;
	Data_type	data_type;
}Fibo;
typedef struct {
	float   *factor_Ptr;
	float   *poollv_Ptr;
	float   *dloss_Ptr;
	Data_type	data_type;
}Ccnstr;
typedef struct {
	int   *jaray_Ptr;
	int   *jkaray_Ptr;
	float   *endlf_Ptr;
	Data_type	data_type;
}Lfcnt;
typedef struct{
	Dev1	dev1; 
	Dev2	dev2; 
	Dev3	dev3;
	Dev4	dev4;
	Data_type	data_type;
}Development;
typedef struct{
	Grow1	grow1;
	Grow2	grow2;
	Grow3	grow3;
	Data_type	data_type;
}Growth;
typedef struct{
	Photo1	photo1;
	Photo2	photo2;
	Data_type	data_type;
}Photosynthesis;
typedef struct{
	Wether		weather;
	Development	development;
	Growth		growth;
	Contrl		control;
	Photosynthesis	photosynthesis;
	Crop		crop;
	Const		_const;
	Rlvsi		rlvsi;
	Rlvsmo		rlvsmo;
	Rlvsto		rlvsto;
	Roots		roots;
	Roots2		roots2;
	Shooti		shooti;
	Shooto		shooto;
	Leap		leap;
	Obs		obs;
	Tilout		tilout;
	Sitvar		sitvar;
	Resp		resp;
	Mslfn		mslfn;
	Fibo		fibo;
	Ccnstr		ccnstr;
	Lfcnt		lfcnt;
	Data_type	data_type;
}AFRC_plant_development_model;

#endif /* _AFRC_GROUPS_H */
