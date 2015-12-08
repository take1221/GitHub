/* Open Com Port  */
/* Return value is TRUE or FALSE */
int Open_Comm1(void);
int Open_Comm2(void);

/* Close Com1 Port  */
void Close_Comm1(void);
void Close_Comm2(void);

/* Read from Com1 Port */
/* int bufnumber_to_read, char *buff */
/* Return value is true read number */
int Read_Comm1(int, unsigned char *);
int Read_Comm2(int, unsigned char *);

/* Write to Com1 Port */
/* int bufnumber_to_write, char *buff */
void Write_Comm1(int, unsigned char *);
void Write_Comm2(int, unsigned char *);

/* Clear Com1 Buffer */
void Clear_Comm1(void);
void Clear_Comm2(void);

/* Write to Com1 Port with Force */
void Write_force_Comm1(unsigned char);
void Write_force_Comm2(unsigned char);
